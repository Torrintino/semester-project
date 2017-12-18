class services_server {

  package { 'libprotobuf10':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'python':
    require => Exec['apt-update'],
    ensure => installed,
  }

  package { 'python-pip':
    require => Exec['apt-update'],
    ensure => installed,
  }

  exec { 'pip-flask':
    require => Package['python-pip'],
    command => '/usr/bin/pip install flask',
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
