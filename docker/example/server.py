#!/usr/bin/python3

import socketserver
import socket
import os

for i in range (1, int(os.environ['clients']) + 1):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('client-{}'.format(i), 5000))
    s.sendall(bytes(b'You are dead'))
    received = str(s.recv(1024))
    print(received)

