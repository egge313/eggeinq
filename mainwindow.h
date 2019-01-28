#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

 private:
    enum QueryStatus
            { QueryUndefined, QueryIdle, QueryStarted,
	      QueryOngoing, QueryHaveResults, QueryFinished };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setStyleSheet (const QString & sheetName);
    void startWorkInAThread ();
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
    void handleResults (const QString & result);
    void handleQueryTimer ();

private:
    Ui::MainWindow *ui;
    const QString version = "0.2.1"; // eggeinq version
    QueryStatus querystatus = QueryIdle;
    QTimer querytimer;
    int queryprogress = 0;
    int querydone = 0;
    //    QMutex querymutex;
    // QWaitCondition querywaitcondition;
};

#endif // MAINWINDOW_H
