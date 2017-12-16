class services_server {

  package { 'libprotobuf10':
    require => Exec['apt-update'],
    ensure => installed,
  }

  file { 'services-server.service':
    path => '/etc/systemd/system/services-server.service',
    ensure => file,
    source => 'puppet:///modules/services_server/services-server.service',
  }

  file { 'services-server':
    path => '/usr/bin/services-server',
    ensure => file,
    source => 'puppet:///modules/services_server/services-server',
  }

  service { 'services-server':
    ensure => running,
    hasrestart => true,
  }

}
