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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnNewProcess_clicked();

    void on_btnBlock_clicked();

    void on_btnTimeSlice_clicked();

    void on_btnTerminate_clicked();

    void on_btnMakeReady_clicked();

private:
    Ui::MainWindow *ui;
    void Scheduler();
    void UpdateReadyList();
    void UpdateRunningList();
    void UpdateBlockedList();
};

#endif // MAINWINDOW_H
