exec { 'apt-update':
  command => '/usr/bin/apt -y update'
}

exec { 'apt-upgrade':
  command => '/usr/bin/apt -y dist-upgrade'
}

include dnsmasq
include hostapd