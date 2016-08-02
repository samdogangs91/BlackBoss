#include "mainwindow.h"
#include <QApplication>
#include "fenclient.h"

using namespace std;

int main(int argc, char *argv[])
{
    //cout << "Hello World!" << endl;
    QApplication a(argc, argv);
    //MainWindow* mw= new MainWindow();
    //mw->show();
    FenClient* cli=new FenClient();
    cli->show();
    return a.exec();
}
