#include <QSizePolicy>
#include <QLayout>

#include "Mainwindow.hpp"
#include "ui_mainwindow.h"
#include "Game.hpp"
#include "Core.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_game(NULL),
    m_board(NULL),
    m_view(NULL)
{
    ui->setupUi(this);
    setMaximumSize(0, 0);
    connect(ui->welcomeCreate, SIGNAL(clicked()),this,SLOT(slotRegister()));
    connect(ui->welcomeConnect, SIGNAL(clicked()),this,SLOT(slotLogin()));
    connect(ui->listCreate, SIGNAL(clicked()), this, SLOT(slotShowGameCreate()));
    connect(ui->createCreate, SIGNAL(clicked()), this, SLOT(slotGameCreate()));
    connect(ui->listRefresh, SIGNAL(clicked()), this, SLOT(slotRefreshList()));
    connect(ui->createPrevious, SIGNAL(clicked()), this, SLOT(slotRefreshList()));
    connect(ui->listConnection, SIGNAL(clicked()), this, SLOT(slotConnection()));
    connect(ui->waitLaunch, SIGNAL(clicked()), this, SLOT(slotLaunchGame()));
}

MainWindow::~MainWindow()
{
    if(m_game)
        delete m_game;
    if(m_view)
        delete m_view;
    if(m_board)
        delete m_board;
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

void MainWindow::listState(QVector<QString> message)
{
    int code = message[0].toInt();
    message.remove(0);
    switch(code)
    {
    case 0:
        setNewGame(message);
        ui->waitName->setText(message[0]);
        ui->waitNbPlayers->setText(message[1]);
        ui->waitSize->setText(message[2]);
        ui->waitNbAlign->setText(message[3]);
        ui->stackedWidget->setCurrentWidget(ui->waitingPage);

        if(Core::Instance().getLogin() == m_game->getAdmin())
            ui->waitLaunch->setDisabled(false);
        break;

    case 1:
        ui->listWidget->setDisabled(false);
        if(ui->stackedWidget->currentWidget() == ui->listPage)
        {
            ui->listState->setText("La partie est pleine.");
        }
        else
        {
            ui->createState->setText("La partie est pleine.");
        }
        break;

    case 2:
        ui->listWidget->setDisabled(false);
        if(ui->stackedWidget->currentWidget() == ui->listPage)
        {
            ui->listState->setText("La partie n'est plus joignable.");
        }
        else
        {
            ui->createState->setText("La partie n'est plus joignable.");
        }
        break;

    case 3:
        ui->listWidget->setDisabled(false);
        if(ui->stackedWidget->currentWidget() == ui->listPage)
        {
            ui->listState->setText("Vous avez déjà rejoint la partie.");
        }
        else
        {
            ui->createState->setText("Vous avez déjà rejoint la partie.");
        }
        break;
    }

}

void MainWindow::waitState(int code)
{
    switch(code)
    {
    case 0:
        ui->waitState->setText("Lancement de la partie.");
        break;

    case 1:
        ui->waitState->setText("Vous n'avez rejoint aucune partie.");
        break;

    case 2:
        ui->waitState->setText("Vous n'etes pas l'administrateur de la partie.");
        break;

    default:
        ui->waitState->setText("Problème serveur.");
    }
}

void MainWindow::setNewGame(QVector<QString> params)
{
    std::string name = params[0].toStdString();
    int nbPlayers = params[1].toInt();
    int size = params[2].toInt();
    int nbAlign = params[3].toInt();
    std::string admin = params[4].toStdString();

    if(m_game)
        delete m_game;

    m_game = new Game(name, nbPlayers, size, nbAlign, admin);
    m_board = new Board(0, m_game);
    m_view = new BoardView(m_board);
    connect(m_board, SIGNAL(play(int,int)), this, SLOT(slotPlay(int, int)));
}

//On récupère la liste de tous les joueurs présents dont le nouveau joueur, c'est plus facile
void MainWindow::addPlayer(QVector<QString> player)
{
    while(ui->waitList->count() != 0)
    {
        QListWidgetItem* item = ui->waitList->takeItem(0);
        delete item;
    }

    for(int i = 0; i < player.count(); i++)
    {
        m_game->addPlayer(player[i]);
        new QListWidgetItem(player[i], ui->waitList);
    }

}

void MainWindow::launchGame()
{
    ui->gamePage->layout()->addWidget(m_view);
    m_view->setMinimumSize(m_game->getSize()*75, m_game->getSize()*75);
    m_view->setMaximumSize(m_game->getSize()*75, m_game->getSize()*75);
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
}

void MainWindow::play(int x, int y, QString player)
{
    m_game->play(x, y, player.toStdString());
    m_board->update();
}

void MainWindow::setGameInfos(QVector<QString> list)
{

    ui->waitName->setText(list[0]);
    ui->waitNbPlayers->setText(list[1]);
    ui->waitSize->setText(list[2]);
    ui->waitNbAlign->setText(list[3]);
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
        ui->createState->setText("Connexion à la partie.");
        qDebug() << ui->createName->text();
        Core::Instance().joinGame(ui->createName->text().toStdString());
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
    ui->listWidget->setDisabled(true);
    //on vérifie qu'une partie a été sélectionnée
    if(item)
    {
        //On récupère le nom de la partie
        QString name = item->text().split("\n")[0];
        Core::Instance().joinGame(name.toStdString());
    }
}

void MainWindow::slotLaunchGame()
{
    Core::Instance().launchGame();
}

void MainWindow::slotPlay(int x, int y)
{
    Core::Instance().play(x, y);
}
