#include "Board.hpp"
#include <QGraphicsPixmapItem>
#include "SquareView.hpp"
#include <QGraphicsSceneEvent>
#include <QTransform>
#include <QDebug>

Board::Board(QWidget* parent, Game *game) :
    QGraphicsScene(parent),
    m_game(game)
{
    int size = m_game->getSize();

    QPainterPath square;
    square.addRoundedRect(0,0,64,64,10,10);

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            SquareView* plop = new SquareView(0, m_game->getSquare(i, j));
            plop->setPath(square);
            plop->setPos(70*i, 70*j);
            plop->setBrush(QBrush(QColor(181,230,85)));
            plop->setPen(QPen(QColor(181,230,85)));
            addItem(plop);
        }
    }
}


void Board::update()
{
    clear();
    int size = m_game->getSize();

    QList<QGraphicsItem*> item = items();
    for(int i=0; i<item.size(); i++)
    {
        removeItem(item[i]);
        delete item[i];
    }


    QPainterPath square_item;
    square_item.addRoundedRect(0,0,64,64,10,10);

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            Square* square = m_game->getSquare(i, j);
            SquareView* plop = new SquareView(0, square);
            plop->setPath(square_item);
            plop->setPos(70*i, 70*j);
            switch(square->getState())
            {
            case 0:
                plop->setBrush(QBrush(QColor(181,230,85)));
                plop->setPen(QPen(QColor(181,230,85)));
                break;

            case 1:
                plop->setBrush(QBrush(QColor(1,176,240)));
                plop->setPen(QPen(QColor(1,176,240)));
                break;

            case 2:
                plop->setBrush(QBrush(QColor(255,53,139)));
                plop->setPen(QPen(QColor(255,53,139)));
                break;

            }

            addItem(plop);
        }
    }

}

void Board::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    SquareView* square = (SquareView*)itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
    if (square && (square->getSquare()->getState() == 0))
        emit play(square->getSquare()->getX(), square->getSquare()->getY());
}
