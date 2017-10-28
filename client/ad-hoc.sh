# join an ad-hoc wifi network

killall wpa_supplicant
iw wlan0 set type ibss
iw wlan0 ibss join coding_tag 2432

