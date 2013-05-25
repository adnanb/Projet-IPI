#!/usr/bin/env python3
#coding=utf-8
import socket, ssl
import time
import psycopg2
import threading
from passlib.hash import pbkdf2_sha1
from passlib.utils import getrandstr, rng, generate_password
from StringUtils import *
from Network import Network

class AuthServer:
    def __init__(self, listening_port, game_server_address, game_server_port):
        localtime = time.asctime(time.localtime(time.time()))
        print("*********************************************")
        print("** Demarrage du serveur d'authentification")
        print("**",localtime)
        print("*********************************************")
        self.listening_port = listening_port
        self.game_server_address = game_server_address
        self.game_server_port = game_server_port


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


    #Que puis-je faire pour vous mon bon monsieur ?
    def handle(self, connection):
        #mess = self.receiveMess(connection)
        mess = connection.receive()
        if mess == -1:
            connection.close()
            return #Le client nous envoie n'importe quoi, on quitte sans lui répondre

        args = StringExtract(mess)
        data = StringBuilder()

        if args[0] == '0':
            res = self.register(args[1], args[2])
            data.add("1")
            data.add(str(res))
            connection.send(data.data.encode())

        if args[0] == '2':
            res = self.login(args[1], args[2])
            data.add("3")
            data.add(str(res))
            if res == 0:
                data.add(str(self.magic_code))
            connection.send(data.data.encode())

        connection.close()



    #On met le monsieur dans la boîte à monsieurs s'il n'existe pas déjà
    def register(self, login, password):
        if (login == '' or password == ''):
            return 2
        try:
            #On se connecte à la boîte à monsieurs
            conn = psycopg2.connect("dbname=puissance5 user=puissance5 password=Disco<3")

            #On vérifie que le monsieur n'est pas déjà dans la boîte
            cur = conn.cursor()
            cur.execute("SELECT * FROM users WHERE login=%s;", (login,))
            if cur.rowcount:
                return 1

            #Le monsieur n'est pas dans la boîte, on commence par hacher son mot de passe avec un algo overkill : PBKDF2_SHA1
            hash = pbkdf2_sha1.encrypt(password)

            #On met le monsieur dans la boîte
            cur.execute("INSERT INTO users (login, password) VALUES (%s, %s);", (login, hash))

            conn.commit()

            cur.close()
            conn.close()

        except Exception as e:
            print("Echec de la création du nouvel utilisateur : \n", e)
            return 3

        return 0

    #Le monsieur est-il bien celui qu'il prétend être ?
    def login(self, login, password):
        if (login == '' or password == ''):
            return 1
        try:
            #On se connecte à la boîte à monsieurs
            conn = psycopg2.connect("dbname=puissance5 user=puissance5 password=Disco<3")

            #On récupère le hash du monsieur
            cur = conn.cursor()
            cur.execute("SELECT password FROM users WHERE login=%s;", (login,))
            if not cur.rowcount:
                return 1

            row = cur.fetchone()
            verify = pbkdf2_sha1.verify(password, row[0])
            if not verify:
                return 1

            cur.close()
            conn.close()

        except Exception as e:
            print("Echec de l'authentification du client : \n", e)
            return 4

        #magic_code = getrandstr(rng, count=20)
        self.magic_code = generate_password(size=20)
        sb = StringBuilder()
        sb.add("4")
        sb.add(str(self.magic_code))
        game_server = Network(False)
        game_server.setHostAddress(self.game_server_address, self.game_server_port)
        game_server.connectToHost()
        game_server.send(sb.data.encode())
        res = game_server.receive()
        game_server.close()
        return 0

app = AuthServer(10000, "localhost", 10001)
app.run()


