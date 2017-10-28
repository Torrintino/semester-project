# join an ad-hoc wifi network

read -p "WLAN device name: " WLAN
read -p "Client ID: " CLIENT_ID

killall wpa_supplicant
iw $WLAN set type ibss
ifconfig $WLAN up
iw $WLAN ibss join coding_tag 2432

ip addr add 192.168.1.$CLIENT_ID/24 broadcast 192.168.1.255 dev $WLAN
