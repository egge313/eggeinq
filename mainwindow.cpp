#include "mainwindow.h"
#include "curlget.h"
#include "ui_mainwindow.h"
#include "osrelease.h"
#include <QFile>
#include <QString>
#include <QProcessEnvironment>
#include <QList>
#include <QNetworkInterface>
#include "workerthread.h"
#include <QDebug>
#include <QThread>
#include <QNetworkInformation>
#include <QHostInfo>

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

  // Progress bar

  ui->progressBar->hide();

  // Checkboxes

  connect(ui->checkBoxIPv4, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedIPv4()));
  connect(ui->checkBoxLocal, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedLocal()));
  connect(ui->checkBoxEnv, SIGNAL(stateChanged(int)), this,
	    SLOT(OnStateChangedEnv()));
  connect(ui->checkBoxCredits, SIGNAL(stateChanged(int)), this,
        SLOT(OnStateChangedCredits()));
  connect(ui->checkBoxOsRelease, SIGNAL(stateChanged(int)), this,
        SLOT(OnStateChangedOsRelease()));


  QString presentation = "<b>Egge's Inquirer (eggeinq) Version " +
                               version + "</b>";
  ui->textEdit->append (presentation);

  // Greet the user
  //
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

  // Connect status.
  if (env.contains("USER"))
    {
      QString greeting = "Hello, " + env.value("USER") + "!";
      ui->textEdit->append (greeting);
      QString localhostname = QHostInfo::localHostName();
      ui->textEdit->append ("Local host name: " + localhostname + ".");
    }
  else
    {
      ui->textEdit->append ("Hello, stranger!");
      ui->connectLabel->setText("   Internet: Not Connected");
    }

  /*
  if (isonline())
    {
      ui->textEdit->append ("You are online.");
      ui->connectLabel->setText("   Internet: Connected");
    }
  else
    {
      ui->textEdit->append ("You are not connected to the internet.");
      ui->connectLabel->setText("   Internet: Not connected");
    }
  */
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
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  if (env.contains("USER"))
    {
      QString greeting = "Bye, " + env.value("USER") + "!";
      ui->textEdit->append (greeting);
    }
  else
    {
      ui->textEdit->append ("Bye, stranger!");
    }
  QThread::msleep(500);
  close();
}

void MainWindow::OnClickedInquire()
{
  if (ui->checkBoxIPv4   -> isChecked())
    {
      querystatus = QueryStarted;
      querytimer.singleShot(1000, this, SLOT(handleQueryTimer()));

      // IPv4 query is performed in a thread
      //
      startWorkInAThread ();

      // Check the system's understanding of its own connectedness status.
      //
      if (isonline())
	{
	  ui->textEdit->append ("You appear to be online.");
	  ui->connectLabel->setText("   Internet: Connected");
	}
      else
	{
	  ui->textEdit->append ("You are not connected to the internet.");
	  ui->connectLabel->setText("   Internet: Not connected");
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
  if (ui->checkBoxOsRelease -> isChecked())
    {
      QStringList releaseinfo;
      os_release (releaseinfo);
      ui->textEdit->append("<h3>Operating system release</h3>");
      for (int i = 0; i < releaseinfo.size(); ++i)
        {
             ui->textEdit->append (releaseinfo.at (i) );
        }
    }
  if (ui->checkBoxCredits   -> isChecked())
    {
      ui->textEdit->append("<h3>Credits</h3>");
      ui->textEdit->append("Programmer: Esa Kettunen");
      ui->textEdit->append("Organization: Egge Collective");
      ui->textEdit->append("Origin: Forssa, Finland, 2018-2025");
      ui->textEdit->append("Contact: esa.kettunen@gmx.com");
      ui->textEdit->append("Source code: https://github.com/egge313/eggeinq");
      ui->textEdit->append("Copyright © 2018-2025 Esa Kettunen");
      ui->textEdit->append("<b>eggeinq<\b> is free software and freely distributed under GNU Lesser Public License (LPGL), Version 3.");
    }
}

void MainWindow::OnStateChangedIPv4()
{
  if (!ui->checkBoxIPv4  -> isChecked() &&
      !ui->checkBoxLocal -> isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxOsRelease -> isChecked() &&      
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
      !ui->checkBoxOsRelease -> isChecked() &&      
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
      !ui->checkBoxOsRelease -> isChecked() &&      
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
      !ui->checkBoxOsRelease -> isChecked() &&      
      !ui->checkBoxEnv->isChecked())
    {
      ui->pushButtonInquire->setEnabled(false);
    }
  else
    {
      ui->pushButtonInquire->setEnabled(true);
    }
}

void MainWindow::OnStateChangedOsRelease()
{
  if (!ui->checkBoxIPv4->isChecked() &&
      !ui->checkBoxLocal->isChecked() &&
      !ui->checkBoxCredits -> isChecked() &&
      !ui->checkBoxOsRelease -> isChecked() &&      
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

// The IPv4 query is in its own thread because e.g. on disconnection there
// may be a significant delay.

void MainWindow::startWorkInAThread ()
{
    WorkerThread *workerThread = new WorkerThread;
    connect(workerThread, &WorkerThread::resultReady, this,
	    &MainWindow::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread,
	    &QObject::deleteLater);
    workerThread->start();
}

void MainWindow::handleResults (const QString & result)
{
  querystatus = QueryHaveResults;

  QThread::yieldCurrentThread();

  //  qDebug() << "QueryHaveResults";

  for (int j = 0; (j * 200) < 1210; ++j)
    {
      if (querydone)
	{
	  break;
	}
      QThread::msleep(200);
    }
  querydone = 0;
  ui->textEdit->append ("<h3>IP number (IPv4):</h3>");
  ui->textEdit->append (result);  
  QThread::yieldCurrentThread();
}

void MainWindow::handleQueryTimer ()
{
  //  qDebug() << "querystatus=" << (int)querystatus;

  switch (querystatus)
    {
    case QueryUndefined:
        ui->textEdit->append ("Error: query undefined.");
	break;
    case QueryIdle:
        ui->textEdit->append ("Error: query idle.");
	break;
    case QueryStarted:
	querystatus = QueryOngoing;
	queryprogress = 1;
	querytimer.singleShot(1000, this, SLOT(handleQueryTimer()));
	break;
    case QueryOngoing:
	if (queryprogress < 99)
	  {
	    ++queryprogress;
	  }
	else
	  {
	    queryprogress = 99;
	  }
	ui->progressBar->setValue(queryprogress);
        ui->progressBar->show();      
	QThread::msleep(2);
	querytimer.singleShot(1000, this, SLOT(handleQueryTimer()));
	break;
    case QueryHaveResults:
    case QueryFinished:
      if (queryprogress > 1)
	{
	  int step = (100 - queryprogress) / 9;
	  for (int i = 1; i <= 10; ++i)
	    {
	      queryprogress += step + 1;
	      if (queryprogress > 99)
		{
		  queryprogress = 99;
		}
	      ui->progressBar->setValue(queryprogress);
	      ui->progressBar->show();      
	      QThread::msleep(150);
	    }
	  ui->progressBar->setValue(100);
	  ui->progressBar->show();      
	  QThread::msleep(500);
	  queryprogress = 0;
	  querytimer.stop();
	  ui->progressBar->hide();
	  querystatus = QueryFinished;
	  querydone = 1;
	}
      break;
    }
}

bool MainWindow::isonline ()
{
    QNetworkInformation* ni = QNetworkInformation::instance();
    if ( nullptr == ni )
        return false;
    QNetworkInformation::Reachability reachability = ni->reachability();
    if ( reachability == QNetworkInformation::Reachability::Online )
        return true;
    else
        return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
