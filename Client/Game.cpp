#include "Game.hpp"

Game::Game(std::string name, int nbPlayers, int size, int nbAlignments, std::string admin):
    m_name(name),
    m_nbPlayers(nbPlayers),
    m_size(size),
    m_nbAlignments(nbAlignments),
    m_admin(admin)

{

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

void Game::addPlayer(QString player)
{
    /*if(m_players.indexOf(player) == -1)
        m_players.push_back(player);*/
}

void Game::play(int x, int y, std::string player)
{

}

