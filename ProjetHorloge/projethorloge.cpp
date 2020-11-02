#pragma once
#include "projethorloge.h"
#include <qboxlayout.h>
#include <qlcdnumber.h>
#include "qaddhour.h"
#include <qdebug.h>

ProjetHorloge::ProjetHorloge(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    updateManualOffset();
    show();
}

void ProjetHorloge::addHour() {
    QAddHour* newhour = new QAddHour; //Ouvre une fenetre d'ajout
    //Connecte la validation de la cr�ation d'une heure � la fen�tre principale
    connect(newhour, &QAddHour::sendNewHour, this, &ProjetHorloge::addNewHourToList);
    newhour->show(); //Affichage de la fen�tre
}

void ProjetHorloge::acceptSerializer(ISerializer& serializer)
{
    serializer.serialize(*this); //Lecture ou sauvegarde des donn�es
    serializeUpdate();
}

void ProjetHorloge::serializeUpdate() { //Rafra�chissement de l'affichage
    ui.actionAffichage_des_secondes->setChecked(m_bIsSecVisible);
    updateManualOffset();
}

void ProjetHorloge::updateManualOffset() { //Mise � jour de l'affichage du d�calage manuel
    int min = m_nManualOffset / 60;
    int hours = min / 60;
    min = min % 60;
    if (min < 0) min = -min;
    if (m_bIsSecVisible) { //On adapte l'affichage selon la visibilit� des secondes
        int sec = m_nManualOffset % 60;
        if (sec < 0) sec = -sec;
        ui.offsetLabel->setText(tr("Decalage manuel : %1:%2:%3").arg(hours).arg(min).arg(sec));
    }
    else {
        ui.offsetLabel->setText(tr("Decalage manuel : %1:%2").arg(hours).arg(min));
    }
}

void ProjetHorloge::addNewHour(QString name, QByteArray timezoneId) { //Cr�ation d'une heure et ajout � la liste
    //On cr�e l'heure avec sa TimeZone (id) et son nom
    QOneTimeZone* timezone = new QOneTimeZone(this, timezoneId, name, m_bIsSecVisible);
    timezone->offsetHour(m_nManualOffset); //On ajuste l'heure en fonction du d�calage manuel
    ui.hoursLayout->addWidget(timezone); //On ajoute l'heure dans le layout avec la QScrollBar
    //On connecte les fonctions d'�dition, de suppression et de mise � jour d'affichage entre la fen�tre principale et l'heure
    connect(timezone, &QOneTimeZone::changeAllHours, this, &ProjetHorloge::changeAllHours);
    connect(timezone, &QOneTimeZone::deleteThis, this, &ProjetHorloge::deleteHour);
    connect(this, &ProjetHorloge::secVisibleWidgets, timezone, &QOneTimeZone::isSecVisible);
    m_lstTimeZones.push_back(timezone); //On la met dans la liste

    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}

void ProjetHorloge::addNewHourToList(QString name, QByteArray timezoneId) {
    addNewHour(name, timezoneId);
}

void ProjetHorloge::changeAllHours(int manualOffset, QOneTimeZone* origin) //Mise � jour des heures si une d'entre elles a �t� modifi�e
{
    m_nManualOffset += manualOffset; //On met � jour le d�calage manuel
    updateManualOffset();
    for (auto& timezone : m_lstTimeZones) { //Pour chaque heure (sauf celle qui vient d'�tre modifi�e et qui n'a pas besoin d'�tre mise � jour), on met � jour avec le nouveau d�calage
        if (timezone != origin) {
            timezone->offsetHour(manualOffset);
        }
    }

    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}

void ProjetHorloge::deleteHour(QOneTimeZone* origin) { //Suppression d'une heure
    for (auto& timezone : m_lstTimeZones) { 
        if (timezone == origin) { //On la cherche dans la liste et on la ferme
            timezone->close();
            m_lstTimeZones.removeOne(timezone);
            break;
        }
    }

    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}

void ProjetHorloge::resetManualOffset() { //R�initialisation du d�calage manuel
    for (auto& timezone : m_lstTimeZones) {
        timezone->offsetHour(-m_nManualOffset); //On met � jour chaque heure
    }
    m_nManualOffset = 0; //On met � jour le d�calage
    if (m_bIsSecVisible) { //Selon la visibilit� des secondes, on met � jour l'affichage du d�calage
        ui.offsetLabel->setText(tr("Decalage manuel : 0:0:0"));
    }
    else {
        ui.offsetLabel->setText(tr("Decalage manuel : 0:0"));
    }

    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}

void ProjetHorloge::isSecVisible(bool isVisible) { //Changement de la visibilit� des secondes
    m_bIsSecVisible = isVisible;
    updateManualOffset(); //On met � jour l'affichage du d�calage manuel
    emit secVisibleWidgets(isVisible); //On envoie un signal pour les sous-widgets
    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}