#include "mainwindow.h"
#include "ui_connexion.h"
#include <iostream>
#include <mysql/mysql.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui->setupUi(this);
    ok=false;
    //QPushButton* conn= new QPushButton("Connexion",this);
    QObject::connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(connexion()));

    /*MYSQL data;
    string Adr="localhost";
    string dbname="blackIa";
    string login="MBlack";
    string mdp="weyPyjmXvb";

    mysql_init(&data);
    mysql_options(&data, MYSQL_READ_DEFAULT_GROUP,"option");
    if(mysql_real_connect(&data,Adr.c_str(),login.c_str(),mdp.c_str(),dbname.c_str(),0,NULL,0))
    {
        mysql_query(&data,"set names utf8");
        cout<<"connect to database successfully"<<endl;
    }*/
}

void MainWindow::connexion()
{
    if(ui->lineEdit->text().compare("")!=0 && ui->lineEdit_2->text().compare("")!=0)
    {
        ok=true;
        cout<<"hey man!"<<endl;
        /*string query="select";
        MYSQL_RES* result=NULL;
        mysql_query(&data,query.c_str());
        result=mysql_store_result(&data);*/
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
