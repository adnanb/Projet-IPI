#include "Square.hpp"

Square::Square(QObject* parent):
    QGraphicsPathItem()
{
    //addRoundedRect(0,0,64,64,10,10);
}

/*Square::Square(QPainterPath path, QObject* parent):
    QPainterPath(path)
{
    addRoundedRect(0,0,64,64,10,10);
}*/

void Square::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

int Square::getX()
{
    return m_x;
}

int Square::getY()
{
    return m_y;
}
