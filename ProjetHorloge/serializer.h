#pragma once

class ProjetHorloge;
class ISerializer //Interface de s�rialisation
{
public:
    virtual void serialize(ProjetHorloge& mainwindow) = 0;
};