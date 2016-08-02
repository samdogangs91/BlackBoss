#ifndef SERVER_H
#define SERVER_H


#include "User.h"
#include "Event.h"
#include <QWidget>
#include <QtNetwork>
#include "ui_serv.h"
#include "Poco/Activity.h"
#include "Poco/Thread.h"


class Server : public QWidget,private Ui::BlackBoss
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = 0);
    void envoyerATous(QString &message);
    void hasEvents();
    bool hasEvent;
    std::queue<Event*> events;
    void start();
    void stop();
    void Behaviour();

signals:
    void newData(QString message);
public slots:
    void nouvelleConnexion();
    void donneesRecues();
    void deconnexionClient();
    void sendToCli(QString message); //message de blackBoss à envoyer au client

private:
    QLabel *etatServeur;
    QPushButton *boutonQuitter;
    QTcpServer *serveur;
    QList<QTcpSocket *> clients;
    quint16 tailleMessage;
    Ui::BlackBoss* ui;
    Poco::Activity<Server> _activity;
};

#endif // SERVER_H
