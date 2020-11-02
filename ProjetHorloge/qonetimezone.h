#pragma once

#include <QWidget>
#include "ui_qonetimezone.h"

class QOneTimeZone : public QWidget //Widget correspondant � une heure, il poss�de un QMyLCDNumber
{
	Q_OBJECT

public:
	QOneTimeZone(QWidget *parent = Q_NULLPTR);
	//Construction � l'aide d'une TimeZone et d'un nom
	QOneTimeZone(QWidget* parent, QByteArray zoneid, QString name, bool isSecVisible);
	~QOneTimeZone();

	void offsetHour(int offset); //Modification de l'heure par rapport au d�calage manuel

	void setSecVisible(bool isVisible);
	QString getName() { return m_sName; };
	QByteArray getZoneId() { return m_bZoneId; };

	void displayHour(); //Mise � jour de l'affichage de l'heure (du LCDNumber)

private:
	Ui::QOneTimeZone ui;
	//Le widget poss�de un nom et une TimeZone associ�e
	QString m_sName;
	QByteArray m_bZoneId;

public slots:
	void bridgeModified(int manualOffset); //Fais le lien entre la modification du QTimeEdit et la fen�tre principale
	void deleteHandler(); //Suppression de la TimeZone
	void isSecVisible(bool isVisible); //Modification de la visibilit� des secondes

signals:
	void changeAllHours(int manualOffset, QOneTimeZone* timezone); //Signal envoy� � la fen�tre principale dans bridgeModified
	void deleteThis(QOneTimeZone* timezone); //Signal envoy� � la fen�tre principale lors de la suppression
};
