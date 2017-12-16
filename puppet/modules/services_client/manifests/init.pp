class services_client {

  package { 'libprotobuf10':
    require => Exec['apt-update'],
    ensure => installed,
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
  }

  service { 'services-client':
    ensure => running,
    hasrestart => true,
  }

}
