#include "Board.hpp"
#include <QGraphicsPixmapItem>
#include "Square.hpp"
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

    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            Square* plop = new Square();
            plop->setPosition(i, j);
            plop->setPath(square);
            plop->setPos(70*j, 70*i);
            plop->setBrush(QBrush(QColor(171,200,226)));
            plop->setPen(QPen(QColor(171,200,226)));
            //QGraphicsPathItem* item = new QGraphicsPathItem(*plop);
            addItem(plop);
            //addPath(*plop,QPen(QColor(20,255,180)), QBrush(QColor(20,255,180)))->setPos(70*j, 70*i);
            //addPixmap(QPixmap("../Puissance5/assets/green_case.png", 0, Qt::AutoColor).scaled(48,48))->setPos(55*j, 55*i);
            //painter.drawPixmap(100*j,100*i,48,48,QPixmap("../Puissance5/assets/green_case.png", 0, Qt::AutoColor));
        }
    }

    //m_scene->addRect(10,10,10,10, QPen(QColor(255, 0, 100, 255)), QBrush(QColor(255, 0, 100, 255), Qt::SolidPattern));
}


void Board::paintEvent(QPaintEvent* event)
{

}

void Board::createGrid()
{
    /*QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, true);*/
    //addRect(100,100,100,100, QPen(QColor(255, 0, 100, 255)), QBrush(QColor(255, 0, 100, 255), Qt::SolidPattern));
    //m_scene->render(painter);
}


void Board::resizeEvent(QResizeEvent* event)
{
    clear();
    Board::createGrid();
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Square* square = (Square*)itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());
    if(square)
        qDebug() << square->getX();
   //qDebug() << square->getX() << square->getY();
    //qDebug() << event->scenePos().x();

}
