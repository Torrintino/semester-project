class hostapd {

  file { 'hostapd':
    path => '/etc/default/hostapd',
    ensure => file,
    source => 'puppet:///modules/hostapd/hostapd',
    notify => Service['hostapd']
  }

  file { 'hostapd.conf':
    path => '/etc/hostapd/hostapd.conf',
    ensure => file,
    source => 'puppet:///modules/hostapd/hostapd.conf',
    notify => Service['hostapd']
  }

  package { 'hostapd':
    require => Exec['apt-update'],
    ensure => installed,
  }

  service { 'hostapd':
    ensure => running,
    hasrestart => true
  }
}