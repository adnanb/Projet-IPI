#include "Game.hpp"
#include <QDebug>

Game::Game(std::string name, int nbPlayers, int size, int nbAlignmentsMax, std::string admin):
    m_name(name),
    m_nbPlayers(nbPlayers),
    m_size(size),
    m_nbAlignmentsMax(nbAlignmentsMax),
    m_admin(admin)

{
    for(int j = 0; j < size; j++)
    {
        for(int i = 0; i < size; i++)
        {
            Square* square = new Square();
            square->setPosition(i, j);
            m_grid.push_back(square);
        }
    }
}

Game::~Game(void)
{

}

std::string Game::getName()
{
    return m_name;
}

int Game::getNbPlayers()
{
    return m_nbPlayers;
}

int Game::getSize(void)
{
    return m_size;
}

int Game::getNbAlignments()
{
    return m_nbAlignments;
}

std::string Game::getAdmin()
{
    return m_admin;
}

//Pas terrible
Square* Game::getSquare(int x, int y)
{
    return m_grid[x+m_size*y];
}

void Game::addPlayer(QString player)
{

}

void Game::play(int x, int y, std::string player)
{
    Square* square = m_grid[x+m_size*y];
    if(square->getState() == 0)
    {
        square->setState(1);
        check(x, y, player);
    }
}

void Game::diagLeftCheck(int x, int y, std::string player)
{
    //On remonte la diagonale vers la gauche
    int i1 = x, j1 = y;
    int i2 = x, j2 = y;
    while(i1 > 0 && j1 > 0 && (m_grid[(i1-1)+m_size*(j1-1)]->getState() == 1))
    {
        i1--;
        j1--;
    }

    while(i2 < (m_size-1) && j2 < (m_size-1) && (m_grid[(i2+1)+m_size*(j2+1)]->getState() == 1))
    {
        i2++;
        j2++;
    }

    if((i2-i1+1) >= 5)
    {
        for(int i = i1; i<(i1+5); i++)
        {
            Square* square = m_grid[i+m_size*j1];
            square->setState(2);
            square->setPlayer(player);
            j1++;
        }
    }
}

void Game::diagRightCheck(int x, int y, std::string player)
{
    //On remonte la diagonale vers la droite
    int i1 = x, j1 = y;
    int i2 = x, j2 = y;
    while(i1 < (m_size-1) && j1 > 0 && (m_grid[(i1+1)+m_size*(j1-1)]->getState() == 1))
    {
        i1++;
        j1--;
    }

    while(i2 > 0 && j2 < (m_size-1) && (m_grid[(i2-1)+m_size*(j2+1)]->getState() == 1))
    {
        i2--;
        j2++;
    }

    if((i1-i2+1) >= 5)
    {
        for(int j = j1; j<(j1+5); j++)
        {
            Square* square = m_grid[i1+m_size*j];
            square->setState(2);
            square->setPlayer(player);
            i1--;
        }
    }
}

void Game::horizontalCheck(int x, int y, std::string player)
{
    //On remonte la diagonale vers la droite
    int i1 = x;
    int i2 = x;

    while(i1 > 0 && (m_grid[(i1-1)+m_size*y]->getState() == 1))
    {
        i1--;
    }

    while(i2 < (m_size-1) && (m_grid[(i2+1)+m_size*y]->getState() == 1))
    {
        i2++;
    }


    if((i2-i1+1) >= 5)
    {
        for(int i = i1; i<(i1+5); i++)
        {
            Square* square = m_grid[i+m_size*y];
            square->setState(2);
            square->setPlayer(player);
        }
    }
}

void Game::verticalCheck(int x, int y, std::string player)
{
    int j1 = y;
    int j2 = y;

    while(j1 > 0 && (m_grid[x+m_size*(j1-1)]->getState() == 1))
    {
        j1--;
    }

    while(j2 < (m_size-1) && (m_grid[x+m_size*(j2+1)]->getState() == 1))
    {
        j2++;
    }


    if((j2-j1+1) >= 5)
    {
        for(int j = j1; j<(j1+5); j++)
        {
            Square* square = m_grid[x+m_size*j];
            square->setState(2);
            square->setPlayer(player);
        }
    }
}

void Game::check(int x, int y, std::string player)
{
    horizontalCheck(x, y, player);
    verticalCheck(x, y, player);
    diagLeftCheck(x, y, player);
    diagRightCheck(x, y, player);
}

