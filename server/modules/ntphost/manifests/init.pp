class ntphost {

  file { 'ntp.conf':
    path => '/etc/ntp.conf',
    ensure => file,
    source => 'puppet:///modules/ntphost/ntp.conf'
  }

  package { 'ntp':
    require => Exec['apt-update'],
    ensure => installed,
  }
}