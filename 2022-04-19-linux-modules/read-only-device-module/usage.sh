sudo insmod read-only-device.ko
dev="read_only_device"
major="$(grep "$dev" /proc/devices | cut -d ' ' -f 1 | head -1)"
sudo mknod "/dev/$dev" c "$major" 0

# Testing read
sudo cat /dev/$dev
sudo cat /dev/$dev
sudo cat /dev/$dev
sudo cat /dev/$dev

# Testing write
# "/dev/chardev: Permission denied" message should emerge.
sudo echo "Hi" > /dev/$dev
sudo echo "Hi" > /dev/$dev
sudo echo "Hi" > /dev/$dev

sudo rmmod $dev
sudo rm /dev/$dev
sudo dmesg -c

if ! grep -q "$dev" /proc/devices
then
	echo "Test is OK, device was removed"
else
	echo "Test failed, device was not removed"
fi
