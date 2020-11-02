#pragma once

#include <QWidget>
#include "ui_qaddhour.h"

class QAddHour : public QWidget //Fenêtre d'ajout d'heure
{
	Q_OBJECT

public:
	QAddHour(QWidget *parent = Q_NULLPTR);
	~QAddHour();

private:
	Ui::QAddHour ui;
	//Nom de l'heure à ajouter, son ID correspondant à sa TimeZone et son décalage en secondes
	QString m_sName;
	QByteArray m_bZoneId;
	int m_nGMTOffset;

public slots:
	void updateString(QString str); //Mise à jour du nom affiché
	void updateTimeZone(); //Mise à jour de la zone affichée (dans la combobox) ainsi que du décalage par rapport à GMT affiché
	void okClicked(); //L'utilisateur confirme la création

signals:
	void sendNewHour(QString name, QByteArray timezoneId); //Donne l'information de l'heure nouvellement créée à la fenêtre principale
};
