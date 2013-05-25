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

class GameServer:
    def __init__(self, listening_port):
        localtime = time.asctime(time.localtime(time.time()))
        print("*********************************************")
        print("** Demarrage du serveur de jeu")
        print("**",localtime)
        print("*********************************************")
        self.listening_port = listening_port
        self.allowed_users = {}
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

    def joinGame(self, login, game_name):
        game = self.games[game_name]
        if game == None:
            return 2
        if len(game.players) >= game.nb_players:
            return 1
        game.players.append(login)
        return 0


    def dedicated_handle(self, connection, login):
        while True:
            mess = connection.receive()
            game = None
            args = StringExtract(mess)
            sb = StringBuilder()

            if args[0] == '8':
                sb.add("9")
                name = args[1]
                nb_players = int(args[2])
                size = int(args[3])
                nb_alignments = int(args[4])
                game = self.game_create(name, nb_players, size, nb_alignments)
                if game != None:
                    game.admin = login
                    game.players.append(login)
                    sb.add("0")
                    print (self.games)
                else:
                    sb.add("1")
                connection.send(sb.data.encode())

            if args[0] == '19':
                sb.add("24")
                for n, g in self.games.items():
                    sb.add(n)
                    sb.add(str(len(g.players)))
                    sb.add(str(g.nb_players))
                connection.send(sb.data.encode())

            if args[0] == '10':
                sb.add("11")
                sb.add(str(self.joinGame(login, args[1])))
                connection.send(sb.data.encode())


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
            sb.add("7")
            check = self.check_if_allowed(args[1], args[2])
            if check == 1:
                sb.add("0")
                connection.send(sb.data.encode())
                self.dedicated_handle(connection, args[1])
            else:
                sb.add("1")
                connection.send(sb.data.encode())

        
        connection.close()


app = GameServer(10001)
app.run()


