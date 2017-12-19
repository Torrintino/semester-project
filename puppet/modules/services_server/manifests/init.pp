class services_server {

  package { 'libprotobuf10':
    require => Exec['apt-update'],
    ensure => installed,
  }

  file { 'libservices-common.so':
    path => '/usr/lib/libservices-common.so',
    ensure => file,
    source => 'puppet:///modules/services_server/libservices-common.so',
  }

  file { 'services-server.service':
    path => '/etc/systemd/system/services-server.service',
    ensure => file,
    source => 'puppet:///modules/services_server/services-server.service',
  }

  file { 'easy.lua':
    path => '/usr/bin/easy.lua',
    ensure => file,
    source => 'puppet:///modules/services_server/easy.lua',
  }

  file { 'services-server':
    path => '/usr/bin/services-server',
    ensure => file,
    source => 'puppet:///modules/services_server/services-server',
    mode => '0744',
  }

  service { 'services-server':
    ensure => running,
    hasrestart => true,
  }

}
