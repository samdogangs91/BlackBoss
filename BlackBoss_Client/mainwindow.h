#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "ui_connexion.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool ok;
    ~MainWindow();

public slots:
    void connexion();
    
private:
    Ui::fenConnexion *ui;
};

#endif // MAINWINDOW_H
