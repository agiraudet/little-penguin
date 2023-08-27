#!/bin/bash

if [ "$EUID" -ne 0 ]; then
	echo "This script must be run as root"
	exit 1
fi

module_name="fourtytwo"

if ! lsmod | grep -wq "$module_name"; then
	echo "Loading module..."
	insmod "$module_name.ko"
fi

echo "Starting tests"
echo "cat /dev/$module_name:"
cat /dev/$module_name
echo ""
echo "echo \"hello\" > /dev/$module_name (should fail):"
echo "hello" > /dev/$module_name
echo $?
echo "echo \"42\" > /dev/$module_name (should fail):"
echo "42" > /dev/$module_name
echo $?
echo "echo \"agiraude\" > /dev/$module_name (should succed):"
echo "agiraude" > /dev/$module_name
echo $?


echo "unloading module..."
rmmod "$module_name.ko"
