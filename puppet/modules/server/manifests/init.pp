class server {

  package { 'git':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'libprotobuf-dev':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'protobuf-compiler':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'liblirc-dev':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'cmake':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'libevent-dev':
    require => Exec['apt-update'],
    ensure => installed,
  }

  file { 'server-stub':
    ensure => 'file',
    path => '/usr/bin/server-stub',
    source => 'puppet:///modules/server/server-stub',
  }

}