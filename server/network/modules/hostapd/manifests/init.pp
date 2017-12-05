class hostapd {

  file { 'hostapd':
    path => '/etc/default/hostapd',
    ensure => file,
    source => 'puppet:///modules/hostapd/hostapd'
  }

  file { 'hostapd.conf':
    path => '/etc/hostapd/hostapd.conf',
    ensure => file,
    source => 'puppet:///modules/hostapd/hostapd.conf'
  }

  package { 'hostapd':
    require => Exec['apt-update'],
    ensure => installed,
  }

  service { 'hostapd':
    ensure => running,
  }
}