#!/usr/bin/python3

import socket
import socketserver

class MyTCPHandler(socketserver.BaseRequestHandler):

    def handle(self):
        req = self.request.recv(1024).strip()
        if req == b'You are dead':
            self.request.sendall(b'%d is sad' % 
                int(socket.gethostname().split('-')[1]))
        
if __name__ == "__main__":
    HOST, PORT = socket.gethostname(), 5000

    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    server.serve_forever()
