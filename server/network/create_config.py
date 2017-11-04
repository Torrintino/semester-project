#!/usr/bin/python3

import csv

conf = """# /etc/dnsmasq.conf

interface=wlan0
dhcp-range=192.168.1.2,192.168.1.249, 255.255.0.0,24h

local=/tag/
domain=tag
expand-hosts
"""

with open('client_id.csv') as client_file:
    reader = csv.reader(client_file)
    for row in reader:
        conf += '\ndhcp-host={},client-{},192.168.1.{},24h'.format(
            row[1], row[0], row[0])
    conf += '\n'
    
with open('server_id.csv') as server_file:
    reader = csv.reader(server_file)
    for row in reader:
        conf += '\ndhcp-host={},server-{},192.168.1.{},24h'.format(
            row[1], row[0], row[0])
    conf += '\n'

with open('dnsmasq.conf', 'w') as dnsmasq:
    dnsmasq.write(conf)
