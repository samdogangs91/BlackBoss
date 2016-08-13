#include "fenconnexion.h"
#include <QApplication>
#include "fenclient.h"

using namespace std;

int main(int argc, char *argv[])
{
    //cout << "Hello World!" << endl;
    QApplication a(argc, argv);
    fenConnexion* fc= new fenConnexion();
    fc->show();
    FenClient* cli=new FenClient();
    //cli->show();
    return a.exec();
}
