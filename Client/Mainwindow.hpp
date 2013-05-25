#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include "Board.hpp"
#include "Boardview.hpp"
#include "Game.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotLogin();
    void slotRegister();
    void slotGameCreate();

    void slotLoginState(int code);
    void slotRegisterState(int code);

private:
    Ui::MainWindow *ui;
    BoardView* m_view;
    Board* m_board;
    Game* m_game;
};
