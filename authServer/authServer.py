#!/usr/bin/env python3
import socket, ssl
import time
import psycopg2
import threading
from passlib.hash import pbkdf2_sha1
from passlib.utils import rng

class authServer:
    def __init__(self, listening_port, game_server_address):
        localtime = time.asctime(time.localtime(time.time()))
        print("*********************************************")
        print("** Demarrage du serveur d'authentification")
        print("**",localtime)
        print("*********************************************")
        self.listening_port = listening_port
        self.game_server_address = game_server_address

    #Boucle principale du programme qui traite les demandes de connexion
    def run(self):
        #Utilisation du protocole TLSv1 (le top)
        context = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
        #Chargement du certificat
        context.load_cert_chain("cert.pem")
        #Création d'une chaussette
        bindsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        bindsocket.bind(('', self.listening_port))
        while True:
            bindsocket.listen(5)
            #Un monsieur tente d'entrer en contact avec nous !
            newsocket, addr = bindsocket.accept()
            try:
                conn = context.wrap_socket(newsocket, server_side=True)
                print('Connexion de ', addr)
                #On dédie un thread à ce client
                thread = threading.Thread(None, self.handle, None, (conn,))
                thread.start()
            except Exception as e:
                print(e)


    #Reçois un message au format netstring de la forme taille:message
    def receiveMess(self, connection):
        #On commence par récupérer la taille du message
        str = ""
        count = 0
        while True:
            if count > 4:
                #La taille du message fait donc plus de 9999 bytes, faut pas déconner
                return -1

            data = connection.read(1)
            char = bytes.decode(data)
            if char ==':':
                break
            str += char
            count += 1
        #Le message que l'on s'apprète à recevoir contient length caractères
        length = int(str)

        if length > 1024:
            #On va limiter les messages à 1024 bytes
            return -1

        data = connection.read(length)
        str = bytes.decode(data)

        return str

    #Renvoie un tableau correspondant aux différents éléments d'un netstring de la forme taille1:element1,taille2:element2,...
    def netstring2array(self, string):
        array = []
        while string != "":
            l, string = string.split(":", 1)
            array.append(string[:int(l)])
            string = string[int(l)+1:]

        return array


    #Que puis-je faire pour vous mon bon monsieur ?
    def handle(self, connection):
        mess = self.receiveMess(connection)
        if mess == -1:
            return #Le client nous envoie n'importe quoi, on quitte sans lui répondre

        args = self.netstring2array(mess)

        if args[0] == '00':
            res = self.register(args[1], args[2])
            data = "7:1:1,1:%d" % res
            connection.sendall(data.encode())

        if args[0] == '02':
            res = self.login(args[1], args[2])
            data = "7:1:3,1:%d" % res
            connection.sendall(data.encode())



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
            return 2
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
                return 3

            cur.close()
            conn.close()

        except Exception as e:
            print("Echec de l'authentification du client : \n", e)
            return 4

        #ToDo : annoncer au serveur de jeu la venue d'un petit nouveau

        return 0

app = authServer(10000, "localhost")
app.run()


