#pragma once
#include "serializer.h"
#include <qjsonobject.h>
#include "projethorloge.h"


//La sérialisation s'effectue à l'aide de fichiers Json

class CJsonWriter : public ISerializer //Ecriture (sauvegarde des données) dans un fichier Json
{
	QString m_strFileName; //Nom du fichier
	QJsonObject m_mainJsonObject; //"Contenu" du fichier
public :
	CJsonWriter(QString filename);
	~CJsonWriter();
	void serialize(ProjetHorloge& mainwindow) override; //Rentre les données importantes dans le fichier Json
};

class CJsonReader : public ISerializer //Lecture (chargement des données) depuis un fichier Json
{
	QString m_strFileName; //Nom du fichier
	QJsonObject m_mainJsonObject; //"Contenu" du fichier
public:
	CJsonReader(QString strFileName);
	void serialize(ProjetHorloge& mainwindow) override; //Donne les données importantes à la fenêtre principale
};