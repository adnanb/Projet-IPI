#include <QSizePolicy>
#include <QLayout>

#include "Mainwindow.hpp"
#include "ui_mainwindow.h"
#include "Game.hpp"
#include "Core.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_game(new Game(7, 7, 7))
{
    m_board = new Board(0, m_game);
    m_view = new BoardView(m_board);
    ui->setupUi(this);
    ui->gamePage->layout()->addWidget(m_view);
    m_view->setMinimumSize(m_game->getSize()*75, m_game->getSize()*75);
    m_view->setMaximumSize(m_game->getSize()*75, m_game->getSize()*75);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMaximumSize(0, 0);
    connect(ui->welcomeCreate, SIGNAL(clicked()),this,SLOT(slotRegister()));
    connect(ui->welcomeConnect, SIGNAL(clicked()),this,SLOT(slotLogin()));
    connect(ui->listCreate, SIGNAL(clicked()), this, SLOT(slotGameCreate()));
}

MainWindow::~MainWindow()
{
    delete m_board;
    delete m_view;
    delete ui;
}

void MainWindow::slotLogin()
{
    ui->welcomeConnect->setDisabled(true);
    Core::Instance().login(ui->welcomeLoginEdit->text().toStdString(), ui->welcomePassEdit->text().toStdString());
}

void MainWindow::slotGameCreate()
{
    ui->stackedWidget->setCurrentWidget(ui->createPage);
}

void MainWindow::slotLoginState(int code)
{
    ui->welcomeConnect->setDisabled(false);
    switch(code)
    {
    case 0:
        ui->stackedWidget->setCurrentWidget(ui->listPage);
        break;

    case 1:
        ui->welcomeState->setText("Identifiants incorrects.");
        break;

    default:
        ui->welcomeState->setText("Problème serveur.");

    }
}

void MainWindow::slotRegister()
{
    ui->welcomeCreate->setDisabled(true);
    Core::Instance().registerToAuth(ui->welcomeLoginEdit->text().toStdString(), ui->welcomePassEdit->text().toStdString());
}

void MainWindow::slotRegisterState(int code)
{
    ui->welcomeCreate->setDisabled(false);
    switch(code)
    {
    case 0:
        ui->welcomeState->setText("Utilisateur créé avec succès.");
        break;

    case 1:
        ui->welcomeState->setText("Ce pseudo est déjà utilisé.");
        break;

    case 2:
        ui->welcomeState->setText("Tous les champs doivent être renseignés.");
        break;

    default:
        ui->welcomeState->setText("Problème serveur.");

    }
}


