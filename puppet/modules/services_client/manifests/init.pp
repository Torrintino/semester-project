class services_client {

  file { 'libservices-common.so':
    path => '/usr/lib/libservices-common.so',
    ensure => file,
    source => 'puppet:///modules/services_client/libservices-common.so',
  }

  file { 'services-client.service':
    path => '/etc/systemd/system/services-client.service',
    ensure => file,
    source => 'puppet:///modules/services_client/services-client.service',
  }

  file { 'libmysqlclient.so':
    path => '/usr/lib/libmysqlclient.so',
    ensure => file,
    source => 'puppet:///modules/services_client/libmysqlclient.so',
  }

  file { 'libmysqlclient.so.18':
    path => '/usr/lib/libmysqlclient.so.18',
    ensure => file,
    source => 'puppet:///modules/services_client/libmysqlclient.so.18',
  }

  file { 'services-client':
    path => '/usr/bin/services-client',
    ensure => file,
    source => 'puppet:///modules/services_client/services-client',
    mode => '0744',
  }

  service { 'services-client':
    ensure => running,
    hasrestart => true,
    require => Service['hardware-api'],
  }

}
