#ifndef FENCLIENT_H
#define FENCLIENT_H

#include <QWidget>
#include <QtNetwork>
#include "ui_FenClient.h"

class FenClient : public QWidget,private Ui::FenClient
{
    Q_OBJECT
public:
    explicit FenClient(QWidget *parent = 0);
private slots:

        void on_boutonConnexion_clicked();

        void on_boutonEnvoyer_clicked();

        void on_message_returnPressed();

        void donneesRecues();

        void connecte();

        void deconnecte();

        void erreurSocket(QAbstractSocket::SocketError erreur);


    private:

        QTcpSocket *socket; // Représente le serveur

        quint16 tailleMessage;

signals:

public slots:
};

#endif // FENCLIENT_H
