#include "Player.hpp"

Player::Player()
{

}

Player::Player(std::string name):
    m_name(name)
{
}

void Player::setName(std::string name)
{
    m_name = name;
}
