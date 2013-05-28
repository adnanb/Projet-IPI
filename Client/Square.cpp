#include "Square.hpp"

Square::Square():
    m_state(0),
    m_player("")
{
}

void Square::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Square::setState(int state)
{
    m_state = state;
}

void Square::setPlayer(std::string player)
{
    m_player = player;
}

int Square::getX()
{
    return m_x;
}

int Square::getY()
{
    return m_y;
}

int Square::getState()
{
    return m_state;
}

std::string Square::getPlayer()
{
    return m_player;
}
