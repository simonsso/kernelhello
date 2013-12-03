# more info found here http://www.cyberciti.biz/tips/build-linux-kernel-module-against-installed-kernel-source-tree.html

obj-m := kernelhello.o
KDIR := ${HOME}/development/rpi_kernel/linux
PWD := $(shell pwd)
V:= 1

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) CROSS_COMPILE=${CROSS_COMPILE}  modules

clean:
	rm -f *.ko *.o .kernelhello.*.cmd
