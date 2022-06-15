obj-m += ioctl.o
PWD   := $(CURDIR)

all: module userspace

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

userspace:
	gcc ioctl_user_space.c -o ioctl_user_space

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf ioctl_user_space
