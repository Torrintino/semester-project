#!/usr/bin/python3

import socketserver
import socket

CLIENT_ID = socket.gethostname().split('-')[1]

class MyTCPHandler(socketserver.BaseRequestHandler):

    def handle(self):
        self.data = self.request.recv(8).strip().split(b'->')
        marksman = self.data[0]
        target = self.data[1]

if __name__ == "__main__":
    HOST, PORT = '192.168.1.{}'.format(CLIENT_ID), 5000

    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    server.serve_forever()
