#include "../../include/Environnement/server.h"
#include "../../include/IA/other.h"


using namespace std;

extern string servStr;
extern string userStr;
extern string msgTalk;
extern unsigned int sigServ;
extern User* Vous;

Server::Server(QWidget *parent) : QWidget(parent), ui(new Ui::BlackBoss),_activity(this, &Server::Behaviour)
{
    // Création et disposition des widgets de la fenêtre
        //cout<<"yo2"<<endl;
        ui->setupUi(this);
        etatServeur = ui->etatServeur;
        boutonQuitter = ui->boutonQuitter;
        connect(this, SIGNAL(newData(QString)), this, SLOT(sendToCli(QString)));
        //cout<<"server2"<<endl;
        connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));


        // Gestion du serveur

        serveur = new QTcpServer(this);
        //cout<<"server1"<<endl;
        if (!serveur->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585

        {
            // Si le serveur n'a pas été démarré correctement
            cout<<"le serveur n'a pas démarré"<<endl;
            etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());

        }

        else
        {
            cout<<"serveur démarré sur le port: "<<serveur->serverPort()<<"!"<<endl;
            // Si le serveur a été démarré correctement
            etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
            connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));

        }
        tailleMessage = 0;
        hasEvent=false;
        start();
}

void Server::hasEvents() //messages venant de BlackBoss à transmettre au client
{
    while(events.size()>0)
    {
        Event* ev=events.front();
        QString message=ev->cont.c_str();
        cout<<"dans serveur: "<<ev->cont.c_str()<<endl;
        //sendToCli(message);
        emit newData(message);
        events.pop();
    }
}

void Server::start()
{
    _activity.start();
}

void Server::stop()
{
    _activity.stop();
    _activity.wait();
}

void Server::Behaviour()
{
    while(!_activity.isStopped())
    {
        while(hasEvent)
        {
            hasEvents();
        }
        Poco::Thread::sleep(20);
    }
}

/*
 * Message provenant du client à transmettre à BlackBoss
 */
void Server::donneesRecues()
{
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients


        // On détermine quel client envoie le message (recherche du QTcpSocket du client)

        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

        if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode

            return;


        // Si tout va bien, on continue : on récupère le message

        QDataStream in(socket);


        if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer

        {

            if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier

                 return;


            in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère

        }


        // Si on connaît la taille du message, on vérifie si on a reçu le message en entier

        if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode

            return;



        // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !

        QString message;

          in >> message;
        cout<<"message reçu dans le serveur:"<<message.toStdString()<<endl;
        //2: on envoie le message à BlackBoss
        string msgInit=message.toStdString();
        string debut=Vous->pseudo->basicName+":/> ";
        string msg=debut+message.toStdString();
        message=msg.c_str();
        envoyerATous(message);
        eventS(userStr,servStr,msgInit,msgTalk,sigServ);

        // 3 : remise de la taille du message à 0 pour permettre la réception des futurs messages

            tailleMessage = 0;
}

void Server::envoyerATous(QString &message)
{
    // Préparation du paquet

        QByteArray paquet;

        QDataStream out(&paquet, QIODevice::WriteOnly);


        out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille

        out << message; // On ajoute le message à la suite

        out.device()->seek(0); // On se replace au début du paquet

        out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message

        cout<<"à envoyer: "<<message.toStdString()<<endl;

        // Envoi du paquet préparé à tous les clients connectés au serveur

        for (int i = 0; i < clients.size(); i++)

        {

            clients[i]->write(paquet);

        }
}

void Server::sendToCli(QString message)
{
    // Préparation du paquet

        QByteArray paquet;

        QDataStream out(&paquet, QIODevice::WriteOnly);


        out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille

        out << message; // On ajoute le message à la suite

        out.device()->seek(0); // On se replace au début du paquet

        out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message

        cout<<"à envoyer: "<<message.toStdString()<<endl;

        // Envoi du paquet préparé à tous les clients connectés au serveur

        for (int i = 0; i < clients.size(); i++)

        {

            clients[i]->write(paquet);

        }
        cout<<"msg envoyé"<<endl;
}

void Server::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();

    clients << nouveauClient;
    cout<<"\n"<<Vous->pseudo->basicName<< " vient de se connecter"<<endl;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));

    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));


}



void Server::deconnexionClient()
{
    //envoyerATous(tr("<em>"+Vous->pseudo->basicName+" vient de se déconnecter</em>"));


    // On détermine quel client se déconnecte

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode

        return;


    clients.removeOne(socket);
    socket->deleteLater();

}



