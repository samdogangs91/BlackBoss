#ifndef FENCONNEXION_H
#define FENCONNEXION_H

#include <QWidget>
#include "ui_fenconnexion.h"
#include <QShortcut>

class fenConnexion: public QWidget, private Ui::fenConnexion
{
    Q_OBJECT
public:
    explicit fenConnexion(QWidget *parent = 0);

public slots:
    void connexion();
    void on_message_return_pressed();
private:
    QShortcut* sc;
};

#endif // FENCONNEXION_H
