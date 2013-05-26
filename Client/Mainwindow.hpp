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

    void refreshList(QVector<QString> list);
    void listState(QVector<QString>);
    void waitState(int code);
    void setGameInfos(QVector<QString> list);
    void setNewGame(QVector<QString> params);
    void addPlayer(QVector<QString> player);
    void launchGame();



public slots:
    void slotLogin();
    void slotRegister();
    void slotShowGameCreate();
    void slotGameCreate();
    void slotRefreshList();
    void slotConnection();
    void slotLaunchGame();
    void slotPlay(int x, int y);

    void slotLoginState(int code);
    void slotRegisterState(int code);
    void slotCreateState(int code);

private:
    Ui::MainWindow *ui;
    BoardView* m_view;
    Board* m_board;
    Game* m_game;
};
