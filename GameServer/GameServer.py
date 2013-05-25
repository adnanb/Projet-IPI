#!/usr/bin/env python3
import socket, ssl
import time
import psycopg2
import threading
from passlib.hash import pbkdf2_sha1
from passlib.utils import getrandstr, rng
from StringUtils import *
from Network import Network

class GameServer:
    def __init__(self, listening_port):
        localtime = time.asctime(time.localtime(time.time()))
        print("*********************************************")
        print("** Demarrage du serveur de jeu")
        print("**",localtime)
        print("*********************************************")
        self.listening_port = listening_port
        self.allowed_users = []


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
        mess = connection.receive()
        if mess == -1:
            cnnection.close()
            return #Le client nous envoie n'importe quoi, on quitte sans lui répondre

        args = StringExtract(mess)
        data = StringBuilder()

        if args[0] == '4':
            data.add("5")
            data.add("0")
            connection.send(data.data.encode())
        
        connection.close()


app = GameServer(10001)
app.run()


