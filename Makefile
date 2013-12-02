# more info found here http://www.cyberciti.biz/tips/build-linux-kernel-module-against-installed-kernel-source-tree.html

obj-m := kernelhello.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
