#pragma once
#include <qlcdnumber.h>
#include <QObject>
#include <qdatetime.h>
#include <qtimer.h>

class QMyLCDNumber : public QLCDNumber //Heure affichée au sein d'un widget QOneTimeZone
{
	Q_OBJECT

private:
	
	//Possède un timer et un temps (en fonction de la TimeZone) en secondes
	QTimer* m_timer;
	QString m_sDate;
	bool m_bIsSecVisible = true;
	

public:
	QMyLCDNumber(QWidget *parent);
	~QMyLCDNumber();

	QString getDate() { return m_sDate; };
	void setDate(QString date) { m_sDate = date; };

	void setSecVisible(bool isVisible) { m_bIsSecVisible = isVisible; };
	bool isSecVisible() { return m_bIsSecVisible; };

	void changeHour(QByteArray timezoneId); //Mise à jour de l'heure avec le décalage de la TimeZone

	void displayHour(); //Mise à jour de l'affichage de l'heure

public slots:
	void tic(); //Mise à jour à chaque seconde
	void modified(QDateTime datetime); //Modification de l'heure si l'utilisateur utilise le QTimeEdit

signals:
	void hourModified(int manualOffset); //Envoie l'information du QTimeEdit à la modification aux autres heures dans la fenêtre principale
};
