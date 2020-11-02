#include "qmylcdnumber.h"
#include <qdebug.h>
#include <qtimezone.h>

QMyLCDNumber::QMyLCDNumber(QWidget *parent)
	: QLCDNumber(parent)
{
	this->setDigitCount(8); //Affichage de 8 digit => 00:00:00
	m_timer = new QTimer(this); //Création du timer pour mettre à jour chaque seconde
	m_sDate = QDateTime::currentDateTime().toString("hh:mm:ss");
	displayHour(); //Affichage de l'heure
	//Connexion du timer avec la fonction Tic
	connect(m_timer, &QTimer::timeout, this, &QMyLCDNumber::tic);
	m_timer->start(1000); //Le timer fait son timeout toutes les secondes
}

void QMyLCDNumber::displayHour() { //Mise à jour de l'affichage de l'heure
	if (m_bIsSecVisible) { //Affiche au format hh:mm:ss ou hh:mm selon la visibilité des secondes
		this->display(m_sDate);
	}
	else {
		QDateTime datewithoutsec = QDateTime::fromString(m_sDate, "hh:mm:ss");
		this->display(datewithoutsec.toString("hh:mm"));
	}
}

QMyLCDNumber::~QMyLCDNumber()
{
}

void QMyLCDNumber::tic() { //A chaque seconde
	QDateTime newdate = QDateTime::fromString(m_sDate, "hh:mm:ss");
	newdate = newdate.addSecs(1);
	m_sDate = newdate.toString("hh:mm:ss"); //On incrémente la date de 1 seconde
	displayHour(); //Et on affiche la modification
}

void QMyLCDNumber::changeHour(QByteArray timezoneId) { //On met à jour l'heure en fonction de la TimeZone et de son décalage GMT
	QTimeZone zone = QTimeZone(timezoneId);
	int offset = zone.offsetFromUtc(QDateTime::currentDateTime());
	QDateTime newdate = QDateTime::currentDateTime().toUTC().addSecs(offset);
	m_sDate = newdate.toString("hh:mm:ss");
	displayHour();
}

void QMyLCDNumber::modified(QDateTime datetime) { //Si le QTimeEdit est modifié, on modifie l'affichage en conséquence
	QDateTime olddate = QDateTime::fromString(m_sDate, "hh:mm:ss");
	int diffBetweenDates = olddate.time().secsTo(datetime.time()); //Différence entre l'heure précédente et l'heure voulue par l'utilisateur
	QDateTime newdate = datetime;
	m_sDate = newdate.toString("hh:mm:ss");
	displayHour(); //On affiche la nouvelle heure
	emit hourModified(diffBetweenDates); //On envoie le décalage à la fenêtre principale
}
