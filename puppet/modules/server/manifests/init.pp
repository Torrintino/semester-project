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

}