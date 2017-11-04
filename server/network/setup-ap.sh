#!/bin/bash

set -e

apt -y update
apt -y dist-upgrade
apt -y install dnsmasq hostapd python3

systemctl stop dnsmasq
systemctl stop hostapd

mv dnsmasq.conf /etc/dnsmasq.conf

./create-config.py
mv hostapd.conf /etc/hostapd/hostapd.conf
mv hostapd /etc/default/hostapd

service hostapd start
service dnsmasq start

"I recommend to reboot and test"
