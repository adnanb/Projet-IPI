#include "SquareView.hpp"

SquareView::SquareView(QObject* parent, Square* square) :
    QGraphicsPathItem(),
    m_square(square)
{
}

Square* SquareView::getSquare()
{
    return m_square;
}
