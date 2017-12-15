class dnsmasq {

  file { 'dnsmasq.conf':
    path => '/etc/dnsmasq.conf',
    ensure => file,
    source => 'puppet:///modules/dnsmasq/dnsmasq.conf',
    notify => Service['dnsmasq'],
  }
  
  package { 'dnsmasq':
    require => Exec['apt-update'],
    ensure => installed,
  }

  service { 'dnsmasq':
    ensure => running,
    hasrestart => true,
  }
}