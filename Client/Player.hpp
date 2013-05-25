#pragma once

#include <string>

class Player
{
public:
    Player();
    Player(std::string name);

    void setName(std::string name);

private:
    std::string m_name;
};
