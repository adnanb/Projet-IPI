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
    connect(ui->listCreate, SIGNAL(clicked()), this, SLOT(slotShowGameCreate()));
    connect(ui->createCreate, SIGNAL(clicked()), this, SLOT(slotGameCreate()));
    connect(ui->listRefresh, SIGNAL(clicked()), this, SLOT(slotRefreshList()));
    connect(ui->createPrevious, SIGNAL(clicked()), this, SLOT(slotRefreshList()));
    connect(ui->listConnection, SIGNAL(clicked()), this, SLOT(slotConnection()));
}

MainWindow::~MainWindow()
{
    delete m_board;
    delete m_view;
    delete ui;
}

void MainWindow::refreshList(QVector<QString> list)
{
    while(ui->listWidget->count() != 0)
    {
        QListWidgetItem* item = ui->listWidget->takeItem(0);
        delete item;
    }

    int size = list.size();
    for(int i = 0; i<size/3; i++)
    {
        QString str = list[3*i]+"\n      "+list[3*i+1]+"/"+list[3*i+2]+" joueur(s)";
        new QListWidgetItem(str, ui->listWidget);
    }
}

void MainWindow::listState(int code)
{
    if(code == 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->waitingPage);
    }
    else
    {
        ui->listState->setText("La partie n'est plus joignable.");
    }
}

void MainWindow::slotLogin()
{
    ui->welcomeConnect->setDisabled(true);
    Core::Instance().login(ui->welcomeLoginEdit->text().toStdString(), ui->welcomePassEdit->text().toStdString());
}

void MainWindow::slotShowGameCreate()
{
    ui->stackedWidget->setCurrentWidget(ui->createPage);
}

void MainWindow::slotGameCreate()
{
    std::string name = ui->createName->text().toStdString();
    std::string nbPlayers = ui->createNbPlayers->text().toStdString();
    std::string size = ui->createSize->text().toStdString();
    std::string nbAlign = ui->createNbAlign->text().toStdString();
    Core::Instance().gameCreate(name, nbPlayers, size, nbAlign);
}

void MainWindow::slotLoginState(int code)
{
    ui->welcomeConnect->setDisabled(false);
    switch(code)
    {
    case 0:
        ui->stackedWidget->setCurrentWidget(ui->listPage);
        slotRefreshList();
        break;

    case 1:
        ui->welcomeState->setText("Identifiants incorrects.");
        break;

    case 2:
        ui->welcomeState->setText("Connexion au serveur de jeu.");
        break;

    case 3:
        ui->welcomeState->setText("Le serveur de jeu a refusé la connexion.");
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

void MainWindow::slotCreateState(int code)
{
    if(code == 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->waitingPage);
    }
    else
    {
        ui->createState->setText("Création de la partie impossible.");
    }
}

void MainWindow::slotRefreshList()
{
    ui->stackedWidget->setCurrentWidget(ui->listPage);
    Core::Instance().getGamesList();
    /*ui->listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* myAction = new QAction(tr("&My Action"), this);
    ui->listWidget->addAction(myAction);
    ui->listWidget->takeItem(1);

    new QListWidgetItem(tr("Oak\n   dd"), ui->listWidget);*/
}

void MainWindow::slotConnection()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    //on vérifie qu'une partie a été sélectionnée
    if(item)
    {
        //On récupère le nom de la partie
        QString name = item->text().split("\n")[0];
        Core::Instance().joinGame(name.toStdString());
    }
}
