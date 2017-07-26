#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() :
    QMainWindow(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);

    model.initialize();

    ui->tableView->setColumnHidden(model.fieldIndex("id"), true);
    model.setHeaderData(model.fieldIndex("prefix"), Qt::Horizontal, tr("Prefix"));
    model.setHeaderData(model.fieldIndex("entity"), Qt::Horizontal, tr("Entity"));
    model.setHeaderData(model.fieldIndex("deleted"), Qt::Horizontal, tr("Deleted"));
    model.setHeaderData(model.fieldIndex("mixed"), Qt::Horizontal, tr("Mixed"));
    model.setHeaderData(model.fieldIndex("phone"), Qt::Horizontal, tr("Phone"));
    model.setHeaderData(model.fieldIndex("cw"), Qt::Horizontal, tr("CW"));
    model.setHeaderData(model.fieldIndex("data"), Qt::Horizontal, tr("Data"));
    model.setHeaderData(model.fieldIndex("sat"), Qt::Horizontal, tr("Satellite"));
    model.setHeaderData(model.fieldIndex("m160"), Qt::Horizontal, tr("160"));
    model.setHeaderData(model.fieldIndex("m80"), Qt::Horizontal, tr("80"));
    model.setHeaderData(model.fieldIndex("m40"), Qt::Horizontal, tr("40"));
    model.setHeaderData(model.fieldIndex("m30"), Qt::Horizontal, tr("30"));
    model.setHeaderData(model.fieldIndex("m20"), Qt::Horizontal, tr("20"));
    model.setHeaderData(model.fieldIndex("m17"), Qt::Horizontal, tr("17"));
    model.setHeaderData(model.fieldIndex("m15"), Qt::Horizontal, tr("15"));
    model.setHeaderData(model.fieldIndex("m12"), Qt::Horizontal, tr("12"));
    model.setHeaderData(model.fieldIndex("m10"), Qt::Horizontal, tr("10"));
    model.setHeaderData(model.fieldIndex("m6"), Qt::Horizontal, tr("6"));
    model.setHeaderData(model.fieldIndex("m2"), Qt::Horizontal, tr("2"));

    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 200);
    for (int i = 3; i < 20; i++)
        ui->tableView->setColumnWidth(i, 50);

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



