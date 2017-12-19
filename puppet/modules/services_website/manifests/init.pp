class services_website {

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

  file { '/path':
    ensure => directory,
  }

  file { '/path/templates':
    ensure => directory,
  }

  file { 'website.py':
    path => '/path/website.py',
    ensure => file,
    source => 'puppet:///modules/services_website/website.py',
  }

  file { 'rules.html':
    path => '/path/templates/rules.html',
    ensure => file,
    source => 'puppet:///modules/services_website/templates/rules.html',
  }

  file { 'startgame.html':
    path => '/path/templates/startgame.html',
    ensure => file,
    source => 'puppet:///modules/services_website/templates/startgame.html',
  }

  file { 'scoreboard.html':
    path => '/path/templates/scoreboard.html',
    ensure => file,
    source => 'puppet:///modules/services_website/templates/scoreboard.html',
  }

  file { 'services-website.service':
    path => '/etc/systemd/system/services-website.service',
    ensure => file,
    source => 'puppet:///modules/services_website/services-website.service',
  }

  service { 'services-website':
    ensure => running,
    hasrestart => true,
  }

}