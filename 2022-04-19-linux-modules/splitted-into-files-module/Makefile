obj-m += begin.o
obj-m += end.o
obj-m += begin_end.o
begin_end-objs += begin.o end.o

PWD   := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
