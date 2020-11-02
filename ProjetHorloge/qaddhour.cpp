#include "qaddhour.h"
#include <qdebug.h>
#include <qtimezone.h>

QAddHour::QAddHour(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//On met les id de toutes les timezones connues par le système dans une liste
	QList<QByteArray> zoneIds = QTimeZone::availableTimeZoneIds();
	foreach (QByteArray id, zoneIds) { //Pour chaque id, on ajoute la timezone correspondante dans la combobox
		ui.zoneComboBox->addItem(id);
	}
	//On connecte le bouton Ok à la validation de la création
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QAddHour::okClicked);
}

QAddHour::~QAddHour()
{
}

void QAddHour::updateString(QString str) { //Changement du nom
	m_sName = str;
}

void QAddHour::updateTimeZone() { //Mise à jour de la zone sélectionnée ainsi que son décalage par rapport à GMT

	m_bZoneId = ui.zoneComboBox->currentText().toLatin1(); //Mise à jour de la zone sélectionnée
	QTimeZone zone = QTimeZone(m_bZoneId);

	//Mise à jour de l'affichage de son décalage par rapport à GMT
	m_nGMTOffset = zone.offsetFromUtc(QDateTime::currentDateTime()) / 60;
	int GMTOffsetHour = m_nGMTOffset / 60;
	int GMTOffsetMin = m_nGMTOffset % 60;
	QString displayoffset;
	if (m_nGMTOffset < 0) {
		displayoffset = tr("%1:%2").arg(GMTOffsetHour).arg(GMTOffsetMin);
	}
	else {
		displayoffset = tr("+%1:%2").arg(GMTOffsetHour).arg(GMTOffsetMin);
	}


	if (zone.isValid()) { //Par défaut, on met le pays de la TimeZone dans le nom
		ui.lineEdit->setText(QLocale::countryToString(zone.country()));
		ui.offsetLabel->setText(tr("Decalage : GMT %1").arg(displayoffset));
	}
}

void QAddHour::okClicked() { //Lorsque la création est validée, cela envoie l'information à la fenêtre principale
	emit sendNewHour(m_sName, m_bZoneId);
	close(); //La fenêtre d'ajout se ferme
}