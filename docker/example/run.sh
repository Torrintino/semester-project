#!/bin/bash

set -e

echo "Start"

for i in `seq 1 $1`;
do
    echo "Start client $i"
    sudo docker run --name "client-$i" -h "client-$i" --net=coding-tag --rm -d weislenn/example "/coding-tag/client.py"
done

sudo docker run --name "server-1" -h "server-1" -e "clients=$1" --net=coding-tag --rm -it weislenn/example /bin/bash
