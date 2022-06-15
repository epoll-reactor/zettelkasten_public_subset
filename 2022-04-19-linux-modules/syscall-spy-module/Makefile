obj-m += syscall-spy.o
PWD   := $(CURDIR)

all: module usage

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

userspace:
	gcc usage.c -o usage

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf usage *.txt
