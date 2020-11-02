#pragma once
#include "serializationclasses.h"
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonarray.h>

CJsonWriter::CJsonWriter(QString strFileName)
    : m_strFileName(strFileName)
{
}

void CJsonWriter::serialize(ProjetHorloge& mainwindow) //Sauvegarde des donn�es
{
    //On sauvegarde le d�calage manuel et la visibilit� des secondes, directement accessibles depuis la fen�tre principale
    m_mainJsonObject["manualoffset"] = mainwindow.getManualOffset();
    m_mainJsonObject["issecvisible"] = mainwindow.isSecVisible();
    //On met l'id et le nom de chaque timezone de la liste de la fen�tre principale dans un objet que l'on met dans un tableau
    QJsonArray timezoneArray;
    for (auto& timezone : mainwindow.getTimeList()) {
        QJsonObject timezoneData;
        timezoneData["name"] = timezone->getName();
        timezoneData["id"] = QString(timezone->getZoneId());

        timezoneArray.push_back(QJsonValue(timezoneData));
    }
    //On met le tableau dans l'objet Json principal
    m_mainJsonObject["timezones"] = timezoneArray;
}


CJsonWriter::~CJsonWriter()
{
    //On inscrit le contenu de l'objet principal Json dans un fichier de nom "save.json" (g�n�ralement) � la destruction de l'objet (fermeture de l'application)
    QFile file(m_strFileName);
    file.open(QIODevice::WriteOnly);

    QJsonDocument jsonDoc(m_mainJsonObject);
    file.write(jsonDoc.toJson());
}

CJsonReader::CJsonReader(QString strFileName)
    : m_strFileName(strFileName)
{
    //On ouvre le fichier "save.json" (g�n�ralement)
    QFile file(m_strFileName);
    file.open(QIODevice::ReadOnly);
    //On met son contenu dans l'objet principal Json
    m_mainJsonObject = QJsonDocument::fromJson(file.readAll()).object();
}

void CJsonReader::serialize(ProjetHorloge& mainwindow) //Lecture des donn�es
{
    //On utilise les setters de la fen�tre principale pour mettre � jour les donn�es
    mainwindow.setManualOffset(m_mainJsonObject["manualoffset"].toInt()); //Mise � jour du d�calage manuel
    mainwindow.setIsSecVisible(m_mainJsonObject["issecvisible"].toBool()); //Mise � jour de la visibilit� des secondes
    for (auto timezone : m_mainJsonObject["timezones"].toArray()) { //Cr�ation de chaque timezone dans le bon ordre, avec son nom et son fuseau
        QJsonObject realTimezone = timezone.toObject();
        mainwindow.addNewHour(realTimezone["name"].toString(), realTimezone["id"].toString().toUtf8());
    }
}
