#pragma once

#include <QGraphicsView>
#include "Game.hpp"

class Board : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0, Game* game = NULL);

    void createGrid();

    void resizeEvent(QResizeEvent*);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    
signals:
    
public slots:

private:
    virtual void paintEvent(QPaintEvent* event);

private:
    QGraphicsScene* m_scene;
    Game* m_game;
    QPainterPath m_square;
    
};
