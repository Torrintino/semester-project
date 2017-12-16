exec { 'apt-update':
  command => '/usr/bin/apt -y update'
}

exec { 'apt-upgrade':
  command => '/usr/bin/apt -y dist-upgrade'
}

node 'server-1' {
  include systemd
  include dnsmasq
  include hostapd
  include ntpserver
}

node default {
  include client
  include lirc
  include hardware_api
  include ntphost
}