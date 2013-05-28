#pragma once

#include "Square.hpp"
#include <string>

class Square
{
public:
    Square();

    void setPosition(int x, int y);
    void setState(int state);
    void setPlayer(std::string player);
    int getX();
    int getY();
    int getState();
    std::string getPlayer();

private:
    int m_x;
    int m_y;
    int m_state;
    std::string m_player;
};
