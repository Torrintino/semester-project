class lircd {

  package { 'lirc':
    ensure => installed,
  }

  package { 'liblirc-dev':
    ensure => installed,
  }

  file { 'config.txt':
    path => '/boot/config.txt',
    ensure => file,
    source => 'puppet:///modules/lircd/config.txt'
  }

  file { 'modules':
    path => '/etc/modules',
    ensure => file,
    source => 'puppet:///modules/lircd/modules'
  }

  file { 'lirc_options.conf':
    path => '/etc/lirc/lirc_options.conf',
    ensure => file,
    source => 'puppet:///modules/lircd/lirc_options.conf'
  }

  file { 'ir-remote.conf':
    path => '/etc/modprobe/ir-remote.conf',
    ensure => file,
    source => 'puppet:///modules/lircd/ir-remote.conf'
  }

  file { 'lircd.conf':
    path => '/etc/lirc/lircd.conf',
    ensure => file,
    source => 'puppet:///modules/lircd/lircd.conf',
    notify => Service['lircd'],
  }

  service { 'lircd':
    ensure => running,
    hasrestart => true,
  }

}
