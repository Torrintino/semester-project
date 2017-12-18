class services_client {

  file { 'libservices-common.so':
    path => '/usr/lib/libservices-common.so',
    ensure => file,
    source => 'puppet:///modules/services_server/libservices-common.so',
  }

  file { 'services-client.service':
    path => '/etc/systemd/system/services-client.service',
    ensure => file,
    source => 'puppet:///modules/services_client/services-client.service',
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
