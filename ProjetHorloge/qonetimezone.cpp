#pragma once
#include "qonetimezone.h"
#include <qtimezone.h>
#include "qdebug.h"

QOneTimeZone::QOneTimeZone(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

QOneTimeZone::QOneTimeZone(QWidget* parent, QByteArray zoneid,  QString name, bool isSecVisible)
	: QWidget(parent),
	m_sName(name),
	m_bZoneId(zoneid)
{
	ui.setupUi(this);
	ui.label->setText(m_sName);
	ui.lcdNumber->changeHour(m_bZoneId);
	setSecVisible(isSecVisible);
}

QOneTimeZone::~QOneTimeZone()
{
}

void QOneTimeZone::bridgeModified(int manualOffset) {
	emit changeAllHours(manualOffset, this); //On transmet le d�calage depuis la modification de son LCDNumber � la fen�tre principale (donc aux autres heures)
}

void QOneTimeZone::offsetHour(int offset) { //Mise � jour de de l'affichage de cette heure par rapport � la modification d'une autre
	QDateTime olddate = QDateTime::fromString(ui.lcdNumber->getDate(), "hh:mm:ss");
	QDateTime newdate = olddate.addSecs(offset);
	ui.lcdNumber->setDate(newdate.toString("hh:mm:ss"));
	displayHour();
}

void QOneTimeZone::displayHour() { //Affiche l'heure en tenant compte de la visibilit� des secondes
	if (ui.lcdNumber->isSecVisible()) {
		ui.lcdNumber->display(ui.lcdNumber->getDate());
	}
	else {
		QDateTime datewithoutsec = QDateTime::fromString(ui.lcdNumber->getDate(), "hh:mm:ss");
		ui.lcdNumber->display(datewithoutsec.toString("hh:mm"));
	}
}

void QOneTimeZone::deleteHandler() {
	emit deleteThis(this); //Envoie l'information de la suppression de l'heure � la fen�tre principale
}

void QOneTimeZone::isSecVisible(bool isVisible) { //R�ceptionne la visibilit� des secondes depuis la fen�tre principale
	setSecVisible(isVisible);
}

void QOneTimeZone::setSecVisible(bool isVisible) { //Modifie l'affichage en fonction de cette visibilit�
	ui.lcdNumber->setSecVisible(isVisible);
	if (isVisible) {
		ui.timeEdit->setDisplayFormat("hh:mm:ss");
		ui.lcdNumber->display(ui.lcdNumber->getDate());
	}
	else {
		ui.timeEdit->setDisplayFormat("hh:mm");
		QDateTime datewithoutsec = QDateTime::fromString(ui.lcdNumber->getDate(), "hh:mm:ss");
		ui.lcdNumber->display(datewithoutsec.toString("hh:mm"));
	}
}

