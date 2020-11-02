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
    //Connecte la validation de la création d'une heure à la fenêtre principale
    connect(newhour, &QAddHour::sendNewHour, this, &ProjetHorloge::addNewHourToList);
    newhour->show(); //Affichage de la fenêtre
}

void ProjetHorloge::acceptSerializer(ISerializer& serializer)
{
    serializer.serialize(*this); //Lecture ou sauvegarde des données
    serializeUpdate();
}

void ProjetHorloge::serializeUpdate() { //Rafraîchissement de l'affichage
    ui.actionAffichage_des_secondes->setChecked(m_bIsSecVisible);
    updateManualOffset();
}

void ProjetHorloge::updateManualOffset() { //Mise à jour de l'affichage du décalage manuel
    int min = m_nManualOffset / 60;
    int hours = min / 60;
    min = min % 60;
    if (min < 0) min = -min;
    if (m_bIsSecVisible) { //On adapte l'affichage selon la visibilité des secondes
        int sec = m_nManualOffset % 60;
        if (sec < 0) sec = -sec;
        ui.offsetLabel->setText(tr("Decalage manuel : %1:%2:%3").arg(hours).arg(min).arg(sec));
    }
    else {
        ui.offsetLabel->setText(tr("Decalage manuel : %1:%2").arg(hours).arg(min));
    }
}

void ProjetHorloge::addNewHour(QString name, QByteArray timezoneId) { //Création d'une heure et ajout à la liste
    //On crée l'heure avec sa TimeZone (id) et son nom
    QOneTimeZone* timezone = new QOneTimeZone(this, timezoneId, name, m_bIsSecVisible);
    timezone->offsetHour(m_nManualOffset); //On ajuste l'heure en fonction du décalage manuel
    ui.hoursLayout->addWidget(timezone); //On ajoute l'heure dans le layout avec la QScrollBar
    //On connecte les fonctions d'édition, de suppression et de mise à jour d'affichage entre la fenêtre principale et l'heure
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

void ProjetHorloge::changeAllHours(int manualOffset, QOneTimeZone* origin) //Mise à jour des heures si une d'entre elles a été modifiée
{
    m_nManualOffset += manualOffset; //On met à jour le décalage manuel
    updateManualOffset();
    for (auto& timezone : m_lstTimeZones) { //Pour chaque heure (sauf celle qui vient d'être modifiée et qui n'a pas besoin d'être mise à jour), on met à jour avec le nouveau décalage
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

void ProjetHorloge::resetManualOffset() { //Réinitialisation du décalage manuel
    for (auto& timezone : m_lstTimeZones) {
        timezone->offsetHour(-m_nManualOffset); //On met à jour chaque heure
    }
    m_nManualOffset = 0; //On met à jour le décalage
    if (m_bIsSecVisible) { //Selon la visibilité des secondes, on met à jour l'affichage du décalage
        ui.offsetLabel->setText(tr("Decalage manuel : 0:0:0"));
    }
    else {
        ui.offsetLabel->setText(tr("Decalage manuel : 0:0"));
    }

    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}

void ProjetHorloge::isSecVisible(bool isVisible) { //Changement de la visibilité des secondes
    m_bIsSecVisible = isVisible;
    updateManualOffset(); //On met à jour l'affichage du décalage manuel
    emit secVisibleWidgets(isVisible); //On envoie un signal pour les sous-widgets
    CJsonWriter JsonWriter("save.json"); //On sauvegarde
    acceptSerializer(JsonWriter);
}