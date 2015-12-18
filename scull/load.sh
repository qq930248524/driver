#!/bin/sh
module="scull"
device="scull"
mode="664"

/sbin/insmod ./$module.ko $* || exit 1
rm -f /dev/${device}[0-3]

major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

echo $major
for i in `seq 0 3`;do
	mknod /dev/${device}$i c $major $i
done
