#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_projethorloge.h"
#include <qstandarditemmodel.h>
#include "qonetimezone.h"
#include "serializationclasses.h"
#include "qscrollarea.h"

class ProjetHorloge : public QMainWindow //Fen�tre principale
{
    Q_OBJECT

public:
    ProjetHorloge(QWidget *parent = Q_NULLPTR);

    void acceptSerializer(ISerializer& serializer); //Fonction de sauvegarde/chargement

    int getManualOffset() { return m_nManualOffset; };
    void setManualOffset(int offset) { m_nManualOffset = offset; };

    bool isSecVisible() { return m_bIsSecVisible; };
    void setIsSecVisible(bool isVisible) { m_bIsSecVisible = isVisible; };

    QList<QOneTimeZone*> getTimeList() { return m_lstTimeZones; };


    void addNewHour(QString name, QByteArray timezoneId); //Fonction permettant d'ajouter une heure � la suite des autres

    void serializeUpdate(); //Rafraichissement de la fen�tre principale apr�s chargement d'un fichier de sauvegarde

    void updateManualOffset(); //Mise � jour de l'offset manuel

private:
    Ui::ProjetHorlogeClass ui;
    //La fen�tre principale comprend une liste de widgets QOneTimeZone correspondant � une TimeZone chacun
    QList<QOneTimeZone*> m_lstTimeZones;
    //Modification manuelle de d�calage de l'utilisateur
    int m_nManualOffset = 0;
    //Bool�en permettant de savoir si les secondes sont visibles ou non
    bool m_bIsSecVisible = true;

public slots:
    void addHour(); //Ouvre une fen�tre d'ajout d'heure
    void addNewHourToList(QString name, QByteArray timezoneId); //Slot reprenant la fonction addNewHour
    void changeAllHours(int manualOffset, QOneTimeZone* origin); //Mise � jour des heures apr�s la modification d'une d'entre elles
    void deleteHour(QOneTimeZone* origin); //Suppression d'une heure de la liste
    void resetManualOffset(); //R�initialisation du d�calage manuel
    void isSecVisible(bool isVisible); //Change la visibilit� des secondes

signals:
    void secVisibleWidgets(bool isVisible); //Envoie la visibilit� des secondes aux sous-widgets
};
