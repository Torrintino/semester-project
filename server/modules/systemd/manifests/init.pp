class systemd {

  exec { 'update-rc.d':
    command => '/usr/sbin/update-rc.d networking remove',
  }

  exec { 'resolv-link':
    command => '/bin/ln -sf /run/systemd/resolve/resolv.conf /etc/resolv.conf'
  }

  file { 'eth0.network':
    path => '/etc/systemd/network/eth0.network',
    ensure => file,
    source => 'puppet:///modules/systemd/eth0.network',
  }

  file { 'wlan0.network':
    path => '/etc/systemd/network/wlan0.network',
    ensure => file,
    source => 'puppet:///modules/systemd/wlan0.network',
  }

  service { 'dhcpcd':
    enable => false,
    ensure => 'stopped',
  }

  service { 'systemd-resolved':
    enable => true,
    ensure => running,
  }

  service { 'systemd-networkd':
    enable => true,
    ensure => running,
  }
}