#pragma once
#include "serializer.h"
#include <qjsonobject.h>
#include "projethorloge.h"


//La s�rialisation s'effectue � l'aide de fichiers Json

class CJsonWriter : public ISerializer //Ecriture (sauvegarde des donn�es) dans un fichier Json
{
	QString m_strFileName; //Nom du fichier
	QJsonObject m_mainJsonObject; //"Contenu" du fichier
public :
	CJsonWriter(QString filename);
	~CJsonWriter();
	void serialize(ProjetHorloge& mainwindow) override; //Rentre les donn�es importantes dans le fichier Json
};

class CJsonReader : public ISerializer //Lecture (chargement des donn�es) depuis un fichier Json
{
	QString m_strFileName; //Nom du fichier
	QJsonObject m_mainJsonObject; //"Contenu" du fichier
public:
	CJsonReader(QString strFileName);
	void serialize(ProjetHorloge& mainwindow) override; //Donne les donn�es importantes � la fen�tre principale
};