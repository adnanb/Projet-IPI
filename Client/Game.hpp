#pragma once

#include "Player.hpp"
#include <QVector>

class Game
{
public:
    Game(int size, int alignment, int nbPlayers);
    ~Game(void);

    int getSize(void);
    void addPlayer(Player* player);
    void play(int x, int y, Player* player);

private:
    int m_nbAlignment;
    int m_nbPlayers;
    int m_size;
    //m_grid;
    QVector<Player*> m_players;

};
