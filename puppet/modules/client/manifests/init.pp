class client {

  file { 'wpa_config.sh':
    path => '/home/pi/wpa_config.sh',
    ensure => file,
    source => 'puppet:///modules/client/wpa_config.sh'
  }

  file { 'wpa_supplicant.conf':
    path => '/home/pi/wpa_supplicant.conf',
    ensure => file,
    source => 'puppet:///modules/client/wpa_supplicant.conf'
  }

  file { 'modules':
    path => '/etc/modules',
    ensure => file,
    source => 'puppet:///modules/client/modules'
  }

  file { 'resolved.conf':
    path => '/etc/systemd/resolved.conf',
    ensure => file,
    source => 'puppet:///modules/client/resolved.conf',
    notify => Service['systemd-resolved'],
  }

  file { 'config.txt':
    path => '/boot/config.txt',
    ensure => file,
    source => 'puppet:///modules/client/config.txt'
  }

  service { 'systemd-resolved':
    ensure => running,
    hasrestart => true,
  }

}
