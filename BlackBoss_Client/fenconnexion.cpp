#include "fenconnexion.h"
#include <iostream>
#include <QShortcut>

using namespace std;

fenConnexion::fenConnexion(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    QObject::connect(ConnectButton,SIGNAL(pressed()),this,SLOT(connexion()));
    //sc= new QShortcut(Qt::EnterKeyDefault,this);
    //connect(sc,SIGNAL(activated()),this,SLOT(on_message_return_pressed()));
}

void fenConnexion::connexion()
{
    if(pseudo->text().compare("")!=0 && password->text().compare("")!=0)
    {
        cout<<"hey man!"<<endl;
    }
}


void fenConnexion::on_message_return_pressed()
{
    connexion();
}
