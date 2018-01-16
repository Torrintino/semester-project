#!/bin/bash
# Author: Lennart Weiss
# This script is automatically compiling binaries from the projects git
# repository and copies them, together with configuration files, into the
# servers file system.
set -e

PUPPET_DIR=/etc/puppet/code/environments/production
MODULES=$PUPPET_DIR/modules

cd /home/pi

echo "Build hardware project"
pushd hardware
git pull

echo "Build hardware-api"
pushd api
make > build.log
popd

popd

echo "Build services project"
pushd services
git pull

pushd build
#cmake -DCMAKE_BUILD_TYPE=Release -DDONT_FORK_SERVER=OFF -DDONT_FORK_CLIENT=0FF .. > build.log

echo "Building services-client"
#make -j4 services-client >> build.log

echo "Building services-server"
#make -j4 services-server >> build.log
popd
popd

echo "Deploying systemd services"
pushd hardware
pushd systemd
cp services-website.service /etc/systemd/system/multi-user.target.wants/
cp services-server.service /etc/systemd/system/multi-user.target.wants/
cp services-client.service $MODULES/services_client/files/
cp hardware-api.service $MODULES/hardware_api/files/
popd
popd

echo "Deploying binaries"
pushd hardware
pushd api
cp hardware-api $MODULES/hardware_api/files/
popd
popd

pushd services
pushd build
cp services-server libservices-common.so /usr/bin/
cp services-client libservices-common.so $MODULES/services_client/files/
popd
pushd website
cp website.py /path/website.py
popd
popd

echo "Restarting systemd services"
systemctl daemon-reload
systemctl restart services-server
systemctl restart services-website

echo "Update puppet configuration"
pushd hardware
pushd puppet
./apply-puppet.sh
popd
popd
