#pragma once

#include <QGraphicsPathItem>
#include "Square.hpp"

class SquareView : public QGraphicsPathItem
{
public:
    explicit SquareView(QObject *parent = 0, Square* square = NULL);

    Square* getSquare();

private:
    Square* m_square;
    
};
