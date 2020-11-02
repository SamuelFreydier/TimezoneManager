#pragma once

class ProjetHorloge;
class ISerializer //Interface de sérialisation
{
public:
    virtual void serialize(ProjetHorloge& mainwindow) = 0;
};