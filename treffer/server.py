#!/usr/bin/python3

import socketserver
import socket

CLIENT_ID = socket.gethostname().split('-')[1]

class MyTCPHandler(socketserver.BaseRequestHandler):

    def handle(self):
        self.data = self.request.recv(4).strip()
        print('{} wrote:'.format(self.client_address[0]))
        print(self.data)
        self.request.sendall(self.data)

if __name__ == "__main__":
    HOST, PORT = '192.168.1.{}'.format(CLIENT_ID), 5000

    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    server.serve_forever()
