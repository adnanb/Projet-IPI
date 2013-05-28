#pragma once

#include "Player.hpp"
#include <QVector>
#include <QString>
#include "Square.hpp"

class Game
{
public:
    Game(std::string name, int nbPlayers, int size, int nbAlignmentsMax, std::string admin);
    ~Game(void);

    std::string getName();
    int getNbPlayers();
    int getSize(void);
    int getNbAlignments();
    Square* getSquare(int x, int y);

    std::string getAdmin();
    void addPlayer(QString player);
    void play(int x, int y, std::string player);

private:
    void horizontalCheck(int x, int y, std::string player);
    void verticalCheck(int x, int y, std::string player);
    void diagLeftCheck(int x, int y, std::string player);
    void diagRightCheck(int x, int y, std::string player);
    void check(int x, int y, std::string player);

    std::string m_name;
    int m_nbPlayers;
    int m_size;
    int m_nbAlignmentsMax;
    int m_nbAlignments;
    std::string m_admin;

    std::vector<Square*> m_grid;
    QVector<QString> m_players;

};
