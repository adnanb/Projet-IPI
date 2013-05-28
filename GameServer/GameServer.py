#!/usr/bin/env python3
import socket, ssl
import time
import psycopg2
import threading
from passlib.hash import pbkdf2_sha1
from passlib.utils import getrandstr, rng
from StringUtils import *
from Network import Network
from Game import Game
from Player import Player

class GameServer:
    def __init__(self, listening_port):
        localtime = time.asctime(time.localtime(time.time()))
        print("*********************************************")
        print("** Demarrage du serveur de jeu")
        print("**",localtime)
        print("*********************************************")
        self.listening_port = listening_port
        self.allowed_users = {}
        self.connected_users = {}
        self.games = {}


    #Boucle principale du programme qui traite les demandes de connexion
    def run(self):
        #Création d'une chaussette
        bindsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        bindsocket.bind(('', self.listening_port))
        while True:
            bindsocket.listen(5)
            #Un monsieur tente d'entrer en contact avec nous !
            new_socket, addr = bindsocket.accept()

            try:
                new_client = Network(True)
                new_client.setSocket(new_socket, True)
                print('Connexion de ', addr)
                #On dédie un thread à ce client
                thread = threading.Thread(None, self.handle, None, (new_client,))
                thread.start()
            except Exception as e:
                print(e)

    def add_allowed_user(self, user, magic_code):
        self.allowed_users[user] = magic_code

    def check_if_allowed(self, user, magic_code):
        if self.allowed_users[user] == magic_code:
            return 1
        else:
            return 0

    def game_create(self, name, nb_players, size, nb_alignments):
        if name in self.games:
            return None
        new_game = Game(name, nb_players, size, nb_alignments)
        self.games[name] = new_game
        return new_game

    def join_game(self, player, game_name):
        game = self.games[game_name]
        if game == None:
            return 2
        if len(game.players) >= game.nb_players:
            return 1
        if player.name in game.players:
            return 3
        game.players.append(player)
        return 0

    def send_to_game_players(self, game_name, message):
        game_players = self.games[game_name].players
        for p in game_players:
            p.connection.send(message.data.encode())

    def launch_game(self, player):
        if player.game == None:
            return 1

        if player.game.admin.name != player.name:
            return 2

        player.game.waiting == False
        sb = StringBuilder()
        sb.add("17")
        self.send_to_game_players(player.game.name, sb)
        return 0

    def dedicated_handle(self, player):
        while True:
            mess = player.connection.receive()
            args = StringExtract(mess)
            sb = StringBuilder()

            #Le client veut créer une partie
            if args[0] == '8':
                sb.add("9")
                name = args[1]
                nb_players = int(args[2])
                size = int(args[3])
                nb_alignments = int(args[4])
                player.game = self.game_create(name, nb_players, size, nb_alignments)
                if player.game != None:
                    player.game.admin = player
                    sb.add("0")
                    print (self.games)
                else:
                    sb.add("1")

            #Le client veut obtenir la liste des parties qu'il peut rejoindre
            if args[0] == '20':
                sb.add("24")
                for n, g in self.games.items():
                    sb.add(n)
                    sb.add(str(len(g.players)))
                    sb.add(str(g.nb_players))

            #Le client veut rejoindre une partie
            if args[0] == '10':
                game_name = args[1]
                res = self.join_game(player, game_name)
                sb.add("11")
                if res == 0:
                    #On prévient les autres joueurs
                    sb2 = StringBuilder()
                    sb2.add("13")
                    for p in self.games[game_name].players:
                        sb2.add(p.name)
                    self.send_to_game_players(game_name, sb2)
                    #On répond au joueur qui veut rejoindre la partie
                    sb.add("0")
                    player.game = self.games[game_name]
                    #On envoie les infos de parties
                    sb.add(game_name)
                    sb.add(str(player.game.nb_players))
                    sb.add(str(player.game.size))
                    sb.add(str(player.game.nb_alignments))
                    sb.add(player.game.admin.name)
                else:
                    sb.add(str(res))
                
            if args[0] == '12':
                res = self.launch_game(player)
                sb.add("15")
                sb.add(str(res))

            if args[0] == '14':
                x = args[1]
                y = args[2]
                _player = args[3]
                sb.add("19")
                sb.add(x)
                sb.add(y)
                sb.add(_player)
                self.send_to_game_players(player.game.name, sb)

            player.connection.send(sb.data.encode())



    #Que puis-je faire pour vous mon bon monsieur ?
    def handle(self, connection):
        mess = connection.receive()
        if mess == -1:
            cnnection.close()
            return #Le client nous envoie n'importe quoi, on quitte sans lui répondre

        args = StringExtract(mess)
        sb = StringBuilder()

        if args[0] == '4':
            self.add_allowed_user(args[1], args[2])
            sb.add("5")
            sb.add("0")
            connection.send(sb.data.encode())

        if args[0] == '6':
            login = args[1]
            magic_code = args[2]
            sb.add("7")
            check = self.check_if_allowed(login, magic_code)
            if check == 1:
                sb.add("0")
                connection.send(sb.data.encode())
                del self.allowed_users[login]
                self.connected_users[login] = Player(login, connection)
                player = self.connected_users[login]
                self.dedicated_handle(player)
            else:
                sb.add("1")
                connection.send(sb.data.encode())

        
        connection.close()


app = GameServer(10001)
app.run()


