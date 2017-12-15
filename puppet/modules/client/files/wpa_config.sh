#!/bin/bash
# Small script for easy WiFi configuration

read -p "Enter SSID: " SSID
read -p "Enter password: " PASSWORD

wpa_passphrase $SSID $PASSWORD >> ./wpa_credentials
cat ./wpa_supplicant.conf ./wpa_credentials > /etc/wpa_supplicant/wpa_supplicant.conf
wpa_cli -i wlan0 reconfigure
