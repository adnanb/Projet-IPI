#pragma once

#include "Player.hpp"
#include <QVector>
#include <QString>

class Game
{
public:
    Game(std::string name, int nbPlayers, int size, int nbAlignments, std::string admin);
    ~Game(void);

    std::string getName();
    int getNbPlayers();
    int getSize(void);
    int getNbAlignments();
    std::string getAdmin();
    void addPlayer(QString player);
    void play(int x, int y, std::string player);

private:
    std::string m_name;
    int m_nbPlayers;
    int m_size;
    int m_nbAlignments;
    std::string m_admin;

    //m_grid;
    QVector<QString> m_players;

};
