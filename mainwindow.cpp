#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <fstream>

namespace
{
    std::string value(std::string &line)
    {
        return line.substr(line.find('>') + 1);
    }
}


MainWindow::MainWindow() :
    QMainWindow(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);

    model.initialize();

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
//    emit ui->actionOpen->triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "c:\\ZOWN\\dxcc", tr("Log File (*.adi)"));
//    QString fileName = "c:\\ZOWN\\dxcc\\DXCC_QSLs_20170619_195452.adi"; // TODO:
    std::ifstream inFile;
    inFile.open(fileName.toStdString());
    if (!inFile)
    {
        qDebug() << "Unable to open file " << fileName;
        return;
    }

    // skip header
    std::string line;
    while (std::getline(inFile, line) && line != "<eoh>")
        ;

    // skip empty line
    std::getline(inFile, line);

    int count = 0;

    while (std::getline(inFile, line))
    {
        std::string call = value(line);
        qDebug() << "call: " << QString::fromStdString(call);

        std::getline(inFile, line);
        std::string date = value(line);
        qDebug() << "date: " << QString::fromStdString(date);

        std::getline(inFile, line);
        std::string modeGroup = value(line);
        qDebug() << "mode group: " << QString::fromStdString(modeGroup);

        std::getline(inFile, line);
        std::string mode = value(line);
        qDebug() << "mode: " << QString::fromStdString(mode);

        std::getline(inFile, line);
        std::string band = value(line);
        qDebug() << "band: " << QString::fromStdString(band);

        std::getline(inFile, line);
        std::string dxcc = value(line);
        qDebug() << "dxcc: " << QString::fromStdString(dxcc);

        std::getline(inFile, line);
        std::string country = value(line);
        qDebug() << "country: " << QString::fromStdString(country);

        std::getline(inFile, line);
        std::string deleted = value(line);
        qDebug() << "deleted: " << QString::fromStdString(deleted);

        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::getline(inFile, line);

        count++;
    }

    qDebug() << "count: " << count;

    inFile.close();
}

