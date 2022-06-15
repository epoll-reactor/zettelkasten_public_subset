sudo insmod sysfs-module.ko

sudo cat /sys/kernel/__sysfs_object/sysfs_value # 0 expected
echo 123 | sudo tee /sys/kernel/__sysfs_object/sysfs_value
sudo cat /sys/kernel/__sysfs_object/sysfs_value # 123 expected

sudo rmmod sysfs_module
