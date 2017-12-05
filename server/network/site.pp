exec { 'apt-update':
  command => '/usr/bin/apt -y update'
}

exec { 'apt-upgrade':
  command => '/usr/bin/apt -y dist-upgrade'
}

package { 'dnsmasq':
  require => Exec['apt-update'],
  ensure => installed,
}

package { 'hostapd':
  require => Exec['apt-update'],
  ensure => installed,
}

package { 'python3':
  require => Exec['apt-update'],
  ensure => installed,
}

service { 'hostapd':
  ensure => running,
}

service { 'dnsmasq':
  ensure => running,
}