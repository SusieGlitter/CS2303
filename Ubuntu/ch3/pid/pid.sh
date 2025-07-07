#!/bin/bash

proc_name="pid"

set -x
make
sudo dmesg -c
sudo insmod ${proc_name}.ko
sudo dmesg

pid=`ps | awk 'NR==2 {print $1}'`
echo pid: ${pid}
echo ${pid} > /proc/${proc_name}
cat /proc/${proc_name}

sudo rmmod ${proc_name}.ko
sudo dmesg
