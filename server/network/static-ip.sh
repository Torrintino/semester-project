#!/bin/bash

set -e

update-rc.d networking remove
systemctl stop dhcpcd
systemctl disable dhcpcd

systemctl enable systemd-resolved
systemctl start systemd-resolved

ln -sf /run/systemd/resolve/resolv.conf /etc/resolv.conf

mv *.network /etc/systemd/network/

systemctl enable systemd-networkd
systemctl start systemd-networkd

echo "Please reboot the system"
