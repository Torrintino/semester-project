#!/usr/bin/python

import socket
import sys

assert(len(sys.argv) == 2)
target = sys.argv[1]

HOST, PORT = '192.168.1.{}'.format(target), 5000
data = '{}->{}'.format(socket.gethostname().split('-')[1],
                                        target)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(bytes(data + '\n'))
received = str(s.recv(1024))

print("Sent:     {}".format(data))
print("Received: {}".format(received))
