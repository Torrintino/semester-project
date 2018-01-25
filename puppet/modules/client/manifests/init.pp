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

}
