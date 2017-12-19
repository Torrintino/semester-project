class lirc {

  package { 'lirc':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'liblirc-dev':
    require => Exec['apt-update'],
    ensure => installed,
  }

  file { 'lirc_options.conf':
    path => '/etc/lirc/lirc_options.conf',
    ensure => file,
    source => 'puppet:///modules/lirc/lirc_options.conf'
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
