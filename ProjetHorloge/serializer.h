#pragma once

class ProjetHorloge;
class ISerializer //Interface de sÚrialisation
{
public:
    virtual void serialize(ProjetHorloge& mainwindow) = 0;
};