#include "qaddhour.h"
#include <qdebug.h>
#include <qtimezone.h>

QAddHour::QAddHour(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//On met les id de toutes les timezones connues par le syst�me dans une liste
	QList<QByteArray> zoneIds = QTimeZone::availableTimeZoneIds();
	foreach (QByteArray id, zoneIds) { //Pour chaque id, on ajoute la timezone correspondante dans la combobox
		ui.zoneComboBox->addItem(id);
	}
	//On connecte le bouton Ok � la validation de la cr�ation
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QAddHour::okClicked);
}

QAddHour::~QAddHour()
{
}

void QAddHour::updateString(QString str) { //Changement du nom
	m_sName = str;
}

void QAddHour::updateTimeZone() { //Mise � jour de la zone s�lectionn�e ainsi que son d�calage par rapport � GMT

	m_bZoneId = ui.zoneComboBox->currentText().toLatin1(); //Mise � jour de la zone s�lectionn�e
	QTimeZone zone = QTimeZone(m_bZoneId);

	//Mise � jour de l'affichage de son d�calage par rapport � GMT
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


	if (zone.isValid()) { //Par d�faut, on met le pays de la TimeZone dans le nom
		ui.lineEdit->setText(QLocale::countryToString(zone.country()));
		ui.offsetLabel->setText(tr("Decalage : GMT %1").arg(displayoffset));
	}
}

void QAddHour::okClicked() { //Lorsque la cr�ation est valid�e, cela envoie l'information � la fen�tre principale
	emit sendNewHour(m_sName, m_bZoneId);
	close(); //La fen�tre d'ajout se ferme
}