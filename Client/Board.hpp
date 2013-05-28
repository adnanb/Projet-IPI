#pragma once

#include <QGraphicsView>
#include "Game.hpp"

class Board : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0, Game* game = NULL);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void update();
    
signals:
    void play(int x, int y);

private:
    QGraphicsScene* m_scene;
    Game* m_game;
    QPainterPath m_square;
    
};
