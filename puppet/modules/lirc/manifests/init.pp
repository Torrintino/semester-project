class lirc {

  package { 'lirc':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'liblirc-dev':
    require => Exec['apt-update'],
    ensure => installed,
  }

  file { 'hardware.conf':
    path => '/etc/lirc/hardware.conf',
    ensure => file,
    source => 'puppet:///modules/lirc/hardware.conf'
  }

  exec { 'old2new':
    command => '/usr/share/lirc/lirc-old2new',
    creates => '/etc/lirc/lirc_options.conf',
  }

  file { 'lircd.conf':
    path => '/etc/lirc/lircd.conf',
    ensure => file,
    source => 'puppet:///modules/lirc/lircd.conf',
    notify => Service['lircd'],
  }

  service { 'lircd':
    ensure => running,
    hasrestart => true,
  }

}
