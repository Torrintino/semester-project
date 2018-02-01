class hardware_api {

  package { 'libprotobuf10':
    ensure => installed,
  }

  package { 'liblirc0':
    ensure => installed,
  }

  file { 'hardware-api.service':
    path => '/etc/systemd/system/hardware-api.service',
    ensure => file,
    source => 'puppet:///modules/hardware_api/hardware-api.service',
  }

  file { 'hardware-api':
    path => '/usr/bin/hardware-api',
    ensure => file,
    source => 'puppet:///modules/hardware_api/hardware-api',
    mode => '0744',
  }

  service { 'hardware-api':
    ensure => running,
    hasrestart => true,
  }

}
