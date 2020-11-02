#pragma once

#include <QWidget>
#include "ui_qaddhour.h"

class QAddHour : public QWidget //Fen�tre d'ajout d'heure
{
	Q_OBJECT

public:
	QAddHour(QWidget *parent = Q_NULLPTR);
	~QAddHour();

private:
	Ui::QAddHour ui;
	//Nom de l'heure � ajouter, son ID correspondant � sa TimeZone et son d�calage en secondes
	QString m_sName;
	QByteArray m_bZoneId;
	int m_nGMTOffset;

public slots:
	void updateString(QString str); //Mise � jour du nom affich�
	void updateTimeZone(); //Mise � jour de la zone affich�e (dans la combobox) ainsi que du d�calage par rapport � GMT affich�
	void okClicked(); //L'utilisateur confirme la cr�ation

signals:
	void sendNewHour(QString name, QByteArray timezoneId); //Donne l'information de l'heure nouvellement cr��e � la fen�tre principale
};
