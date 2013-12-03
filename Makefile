# more info found here http://www.cyberciti.biz/tips/build-linux-kernel-module-against-installed-kernel-source-tree.html

obj-m := kernelhello.o
KDIR := /lib/modules/3.8.0-26-generic/build/
# KDIR := ${HOME}/development/rpi_kernel/linux
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) CROSS_COMPILE=${CROSS_COMPILE}  modules

clean:
	rm -f *.ko *.o .kernelhello.*.cmd
