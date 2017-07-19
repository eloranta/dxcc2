#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Model model;
public:
    explicit MainWindow();
    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void fileOpen();
};

#endif // MAINWINDOW_H
