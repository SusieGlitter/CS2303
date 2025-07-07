#!/bin/bash

proc_name="jiffies"

set -x
make
sudo dmesg -c
sudo insmod ${proc_name}.ko
sudo dmesg
cat /proc/${proc_name}
sleep 1s
cat /proc/${proc_name}
sleep 2s
cat /proc/${proc_name}
sudo rmmod ${proc_name}.ko
sudo dmesg
