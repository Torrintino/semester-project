class ntpserver {

  file { 'ntp.conf':
    path => '/etc/ntp.conf',
    ensure => file,
    source => 'puppet:///modules/ntpserver/ntp.conf'
  }

  package { 'ntp':
    require => Exec['apt-update'],
    ensure => installed,
  }
}