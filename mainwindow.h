#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setStyleSheet(const QString & sheetName);
    ~MainWindow();

private:
    void showhostip();

public slots:
    void OnClickedClose();
    void OnClickedInquire();
    void OnStateChangedIPv4();
    void OnStateChangedLocal();    
    void OnStateChangedEnv();
    void OnStateChangedCredits();

private:
    Ui::MainWindow *ui;
    QString version = "0.1.5"; // eggeinq version
};

#endif // MAINWINDOW_H
