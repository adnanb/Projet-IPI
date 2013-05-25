from StringUtils import StringExtract
import socket
import ssl

class Network:
    def __init__(self, server_side):
        self.address = ""
        self.port = 0
        self.blockSize = 0
        self.data = ""
        new_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #Utilisation du protocole TLSv1 (le top)
        self.context = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
        #Chargement du certificat
        self.context.load_cert_chain("cert.pem")
        self.socket = self.context.wrap_socket(new_socket, server_side)

    def setSocket(self, socket, server_side):
        self.socket = self.context.wrap_socket(socket, server_side)


    def connectToHost(self):
        self.socket.connect((self.address, self.port))

    def close(self):
        self.socket.shutdown(socket.SHUT_RDWR)
        self.socket.close()

    def setHostAddress(self, address, port):
        self.address = address
        self.port = port



    def receive(self):
        #On commence par récupérer la taille du message
        str = ""
        count = 0
        data = b""
        while True:
            if count > 4:
                #La taille du message fait donc plus de 9999 bytes, peu probable
                return -1

            data = self.socket.recv(1)
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

        data = self.socket.recv(length)

        return data.decode()

    def send(self, message):
        self.socket.sendall(message)



