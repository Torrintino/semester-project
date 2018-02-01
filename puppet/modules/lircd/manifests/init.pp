class lirc {

  package { 'lirc':
    ensure => installed,
  }

  package { 'liblirc-dev':
    ensure => installed,
  }

  file { 'config.txt':
    path => '/boot/config.txt',
    ensure => file,
    source => 'puppet:///modules/lirc/config.txt'
  }

  file { 'modules':
    path => '/etc/modules',
    ensure => file,
    source => 'puppet:///modules/lirc/modules'
  }

  file { 'lirc_options.conf':
    path => '/etc/lirc/lirc_options.conf',
    ensure => file,
    source => 'puppet:///modules/lirc/lirc_options.conf'
  }

  file { 'ir-remote.conf':
    path => '/etc/modprobe/ir-remote.conf',
    ensure => file,
    source => 'puppet:///modules/lirc/ir-remote.conf'
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
