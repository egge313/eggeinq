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

public slots:
    void OnClickedClose();
    void OnClickedInquire();
    void OnStateChangedIPv4();
    void OnStateChangedLocal();    
    void OnStateChangedEnv();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
