#include "mainwindow.h"
#include "curlget.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QProcessEnvironment>
#include <QList>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setStyleSheet("coffee");

  // Buttons

  connect(ui->pushButtonClose, SIGNAL(clicked()), this,
	    SLOT(OnClickedClose()));
  connect(ui->pushButtonInquire, SIGNAL(clicked()), this,
	    SLOT(OnClickedInquire()));  
  ui->pushButtonInvisible1->hide();
  ui->pushButtonInvisible2->hide();

  // Checkboxes

  connect(ui->checkBoxIPv4, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedIPv4()));
  connect(ui->checkBoxLocal, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedLocal()));
  connect(ui->checkBoxEnv, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedEnv()));
  connect(ui->checkBoxCredits, SIGNAL(stateChanged(int)), this,
        SLOT(OnStateChangedCredits()));


  QString presentation = "<b>Egge's Inquirer (eggeinq) Version " + version + "</b>";
  ui->textEdit->append (presentation);

  // Greet the user
  //
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  if (env.contains("USER"))
    {
      QString greeting = "Hello, " + env.value("USER") + "!";
      ui->textEdit->append (greeting);
    }
  else
    {
      ui->textEdit->append ("Hello, stranger!");
    }

  // Check the IPv4 box 
  //
  ui->checkBoxIPv4->setCheckState(Qt::Checked);

  OnClickedInquire();

  // set application icon
  QIcon windowicon(":/images/icon.png");
  setWindowIcon(windowicon);

}

void MainWindow::OnClickedClose()
{
  close();
}

void MainWindow::OnClickedInquire()
{
  if (ui->checkBoxIPv4   -> isChecked())
    {
      QString data;
      QString errmsg;
      if (curlget (data, errmsg))
	{
	  ui->textEdit->append ("<h3>IP number (IPv4):</h3>");
	  ui->textEdit->append (data);
	}	
      else
	{
          ui->textEdit->append (errmsg);
          ui->textEdit->append ("Are you sure your computer is connected to the Internet?");
	}	

    }
  if (ui->checkBoxLocal   -> isChecked())
    {
	  ui->textEdit->append (
			 "<h3>Local network addresses in this host:</h3>");
	  showhostip ();
    }
  if (ui->checkBoxEnv   -> isChecked())
    {
      ui->textEdit->append("<h3>Environment variables</h3>");

      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      QStringList keys_values = env.toStringList();
      for (int i = 0; i < keys_values.size(); ++i)
	{
          ui->textEdit->append (keys_values.at(i).toLocal8Bit().constData());
	}
    }
  if (ui->checkBoxCredits   -> isChecked())
    {
      ui->textEdit->append("<h3>Credits</h3>");
      ui->textEdit->append("Programmer: Esa Kettunen");
      ui->textEdit->append("Organization: Egge Collective");
      ui->textEdit->append("Origin: Forssa, Finland, 2018");
      ui->textEdit->append("Contact: esa.kettunen@gmx.com");
      ui->textEdit->append("Source code: https://github.com/egge313/eggeinq");
      ui->textEdit->append("Copyright © Esa Kettunen 2018");
      ui->textEdit->append("eggeinq is free software and freely distributed under GNU Lesser Public License (LPGL)");
    }
}

void MainWindow::OnStateChangedIPv4()
{
  if (!ui->checkBoxIPv4  -> isChecked() &&
      !ui->checkBoxLocal -> isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxEnv   -> isChecked())
    {
      ui->pushButtonInquire->setEnabled(false);
    }
  else
    {
      ui->pushButtonInquire->setEnabled(true);
    }
}

void MainWindow::OnStateChangedLocal()
{
  if (!ui->checkBoxIPv4->isChecked() &&
      !ui->checkBoxLocal->isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxEnv->isChecked())
    {
      ui->pushButtonInquire->setEnabled(false);
    }
  else
    {
      ui->pushButtonInquire->setEnabled(true);
    }
}

void MainWindow::OnStateChangedEnv()
{
  if (!ui->checkBoxIPv4->isChecked() &&
      !ui->checkBoxLocal->isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxEnv->isChecked())
    {
      ui->pushButtonInquire->setEnabled(false);
    }
  else
    {
      ui->pushButtonInquire->setEnabled(true);
    }
}

void MainWindow::OnStateChangedCredits()
{
  if (!ui->checkBoxIPv4->isChecked() &&
      !ui->checkBoxLocal->isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxEnv->isChecked())
    {
      ui->pushButtonInquire->setEnabled(false);
    }
  else
    {
      ui->pushButtonInquire->setEnabled(true);
    }
}


void MainWindow::setStyleSheet(const QString & sheetName)
{
  QFile file(":/qss/" + sheetName.toLower() + ".qss");
  file.open(QFile::ReadOnly);
  QString styleSheet = QString::fromLatin1(file.readAll());
  qApp->setStyleSheet(styleSheet);
}


void MainWindow::showhostip()
{
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
             ui->textEdit->append(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
             ui->textEdit->append(ipAddressesList.at(i).toString());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
