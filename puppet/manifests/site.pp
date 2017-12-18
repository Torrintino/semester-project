exec { 'apt-update':
  command => '/usr/bin/apt -y update'
}

exec { 'apt-upgrade':
  command => '/usr/bin/apt -y dist-upgrade'
}

node 'server-1' {
  include server
  include systemd
  include dnsmasq
  include hostapd
  include ntpserver
  include services_server
  include services_website
}

node default {
  include client
  include ntphost
  include lirc
  include hardware_api
  include services_client
}