#!/bin/bash

module="hello"
device="zjy_"
mode="644"

/sbin/insmod ./$module.ko $* ||exit 1

rm -f /dev/${device}[0-3]

major=$(awk '$2 == "$device" {print $1}' /proc/devices)

mknod /dev/${device}0 c $major 0
mknod /dev/${device}1 c $major 1

group="staff"
grep -q '^staff:' /etc/group || group="wheel"

chgrp $group /dev/${device}[0-3]
chmod $mode /dev/${device}[0-3]
