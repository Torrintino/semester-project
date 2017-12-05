class dnsmasq {
  
  package { 'dnsmasq':
    require => Exec['apt-update'],
    ensure => installed,
  }

  service { 'dnsmasq':
    ensure => running,
  }
}