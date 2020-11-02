#pragma once
#include "projethorloge.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
#include "serializationclasses.h"
#include "qfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjetHorloge w; //Fenêtre principale
    QFile savefile("save.json");
    
    if (savefile.exists()) { //Si le fichier de sauvegarde existe...
        CJsonReader JsonReader("save.json");
        w.acceptSerializer(JsonReader); //On le charge dans la fenêtre principale et on récupère les données
    }

    return a.exec();
}
