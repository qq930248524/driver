#!/bin/bash

test=$(awk '$2 == "zjy_"  {print $1}' /proc/devices)
echo $test
