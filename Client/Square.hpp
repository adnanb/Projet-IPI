#pragma once

#include <QGraphicsPathItem>

class Square : public QGraphicsPathItem
{
public:
    explicit Square(QObject *parent = 0);

    void setPosition(int x, int y);
    int getX();
    int getY();

signals:
    
public slots:

private:
    int m_x;
    int m_y;
};
