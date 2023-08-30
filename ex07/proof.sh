#!/bin/bash

if [[ $EUID -ne 0 ]]; then
				echo "This script must be run as root."
				exit 1
fi

module_name="ft_debug"

if ! lsmod | grep -wq "$module_name"; then
				echo "Loading module..."
				insmod "$module_name.ko"
fi

chmod o+r /sys/kernel/debug/fortytwo

check_command() {
				if [ $? -eq 0 ]; then
								echo "Command: $1 - Passed"
				else
								echo "Command: $1 - Failed"
				fi
}

cat /sys/kernel/debug/fortytwo/id
check_command "cat /sys/kernel/debug/fortytwo/id"
echo 

echo "Test" > /sys/kernel/debug/fortytwo/id
check_command "echo 'Test' > /sys/kernel/debug/fortytwo/id"
echo "(This should failed with \"Invalid argument\")"
echo 

echo "agiraude" > /sys/kernel/debug/fortytwo/id
check_command "echo 'agiraude' > /sys/kernel/debug/fortytwo/id"
echo 

jiffies_value=$(cat /sys/kernel/debug/fortytwo/jiffies)
timer_jiffies=$(grep ^jiffies /proc/timer_list | head -1 | cut -d ' ' -f 2)
echo "cat /sys/kernel/debug/fortytwo/jiffies                      $jiffies_value"
echo "grep ^jiffies /proc/timer_list | head -1 | cut -d ' ' -f 2  $timer_jiffies"
echo "(The two values should be kinda close)"
echo 

echo "Hello, kernel!" > /sys/kernel/debug/fortytwo/foo
check_command "echo 'Hello, kernel!' > /sys/kernel/debug/fortytwo/foo"

foo_content=$(cat /sys/kernel/debug/fortytwo/foo)
echo "-> $foo_content"
if [ "$foo_content" == "Hello, kernel!" ]; then
				echo "Command: cat /sys/kernel/debug/fortytwo/foo - Passed"
else
				echo "Command: cat /sys/kernel/debug/fortytwo/foo - Failed"
fi

echo "Tests completed."
