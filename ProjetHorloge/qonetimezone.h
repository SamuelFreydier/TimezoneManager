#pragma once

#include <QWidget>
#include "ui_qonetimezone.h"

class QOneTimeZone : public QWidget //Widget correspondant à une heure, il possède un QMyLCDNumber
{
	Q_OBJECT

public:
	QOneTimeZone(QWidget *parent = Q_NULLPTR);
	//Construction à l'aide d'une TimeZone et d'un nom
	QOneTimeZone(QWidget* parent, QByteArray zoneid, QString name, bool isSecVisible);
	~QOneTimeZone();

	void offsetHour(int offset); //Modification de l'heure par rapport au décalage manuel

	void setSecVisible(bool isVisible);
	QString getName() { return m_sName; };
	QByteArray getZoneId() { return m_bZoneId; };

	void displayHour(); //Mise à jour de l'affichage de l'heure (du LCDNumber)

private:
	Ui::QOneTimeZone ui;
	//Le widget possède un nom et une TimeZone associée
	QString m_sName;
	QByteArray m_bZoneId;

public slots:
	void bridgeModified(int manualOffset); //Fais le lien entre la modification du QTimeEdit et la fenêtre principale
	void deleteHandler(); //Suppression de la TimeZone
	void isSecVisible(bool isVisible); //Modification de la visibilité des secondes

signals:
	void changeAllHours(int manualOffset, QOneTimeZone* timezone); //Signal envoyé à la fenêtre principale dans bridgeModified
	void deleteThis(QOneTimeZone* timezone); //Signal envoyé à la fenêtre principale lors de la suppression
};
