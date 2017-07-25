#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() :
    QMainWindow(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);

    model.initialize();

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileOpen()
{
    model.fileOpen();
}



