#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringListModel>
#include <QList>
#include "processitem.h"

QStringList * readyProcesses;
QStringListModel * readyProcessesModel;
QStringList * running;
QStringListModel * runningModel;
QStringList * blocked;
QStringListModel * blockedModel;

QList<ProcessItem> _ready;
ProcessItem _running;
QList<ProcessItem> _blocked;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //link lists to models and models to list views
    readyProcesses = new QStringList();
    readyProcessesModel = new QStringListModel(*readyProcesses, NULL);
    ui->lstReady->setModel(readyProcessesModel);
    running = new QStringList();
    runningModel = new QStringListModel(*running, NULL);
    ui->lstRunning->setModel(runningModel);
    blocked = new QStringList();
    blockedModel = new QStringListModel(*blocked, NULL);
    ui->lstBlocked->setModel(blockedModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnNewProcess_clicked()
{
    //Make new process from values, check that text box isnt empty
    if (ui->txtNewProcess->text() != "")
    {
        ui->statusBar->clearMessage();
        ProcessItem NewProcess(ui->txtNewProcess->text(),ui->intTimeNeeded->value());
        _ready.append(NewProcess);

        UpdateReadyList();

        //if running is empty then call scheduler to pull it into execution
        if(running->isEmpty())
        {
            Scheduler();
        }
    }
    else
    {
        ui->statusBar->showMessage("Process needs a name", 5000);
    }
}

//Function that looks at the list of processes and takes the next one moving it to in process
void MainWindow::Scheduler()
{
    //pop the top process off of _ready and move it to running, reprint both lists
    if (!_ready.isEmpty())
    {
        //there is items to be scheduled, if not then clear the in process list.
        _running = _ready.takeFirst();

        UpdateRunningList();
        UpdateReadyList();
    }
    else
    {
        //clear the running list by clearing the list and then updating the container
        running->clear();
        runningModel->setStringList(*running);
    }
}

// Move running process to blocked list, call scheduler
void MainWindow::on_btnBlock_clicked()
{
    if(!running->isEmpty())
    {
        ui->statusBar->clearMessage();
        _blocked.append(_running);
        UpdateBlockedList();

        Scheduler();
    }
    else
    {
        ui->statusBar->showMessage("No process running", 5000);
    }
}

// Move running process to ready, decrement time slice
void MainWindow::on_btnTimeSlice_clicked()
{
    if(!running->isEmpty())
    {
        ui->statusBar->clearMessage();
        _running.TimeNeeded -= ui->intTimeSlice->value();

        if (_running.TimeNeeded > 0) //if the process needs more time send it back to running
        {
            _ready.append(_running);
            UpdateReadyList();
        }
        else
        {
            ui->statusBar->showMessage("Process finished within timeslice", 5000);
        }

        Scheduler();
    }
    else
    {
        ui->statusBar->showMessage("No process running", 5000);
    }
}

// Move selected process to ready list
void MainWindow::on_btnMakeReady_clicked()
{
    bool added = false;
    // make list of selected items from listview on form: Should only be one, I am not sure how to multi select
    QModelIndexList ListOfItemsOnForm = ui->lstBlocked->selectionModel()->selectedIndexes();

    foreach (const QModelIndex &selectedIndex, ListOfItemsOnForm)
    {
        // check it against the items in blocked list, if found move it to ready list
        foreach (ProcessItem p, _blocked)
        {
            if (p.Display() == selectedIndex.data(Qt::DisplayRole).toString())
            {
                //this is the item, move it to ready in the right position
                foreach (ProcessItem q, _ready)
                {
                    if (q.UID > p.UID)
                    {
                        _ready.insert(_ready.indexOf(q),p);
                        added = true;
                    }
                }
                if (!added)
                {
                    _ready.append(p);
                }
                //and remove it from blocked
                _blocked.removeOne(p);
            }
        }
    }
    //update everything
    UpdateReadyList();
    UpdateBlockedList();
}

void MainWindow::on_btnTerminate_clicked()
{
    if (!running->isEmpty())
    {
        Scheduler();
    }
    else
    {
        ui->statusBar->showMessage("No process running", 5000);
    }
}

void MainWindow::UpdateReadyList()
{
    readyProcesses->clear();
    foreach (ProcessItem p, _ready) {
        readyProcesses->append(p.Display());
    }
    readyProcessesModel->setStringList(*readyProcesses);
}

void MainWindow::UpdateRunningList()
{
    running->clear();
    running->append(_running.Display());

    runningModel->setStringList(*running);
}

void MainWindow::UpdateBlockedList()
{
    blocked->clear();
    foreach (ProcessItem p, _blocked) {
        blocked->append(p.Display());
    }
    blockedModel->setStringList(*blocked);
}
