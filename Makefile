# 
# Copyright (c) 2020 Xilinx, Inc.
# All rights reserved.
# 
# This source code is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
# 
# The full GNU General Public License is included in this distribution in
# the file called "COPYING".
#
ifdef KVERSION
KERNEL_VERS = $(KVERSION)
else
KERNEL_VERS = $(shell uname -r)
endif

COMPILEDB = $(shell which compiledb)
ONIC_MAX_QUEUES ?= 1

srcdir = $(PWD)
obj-m += onic.o
#BASE_OBJS := $(patsubst $(srcdir)/%.c,%.o,$(wildcard $(srcdir)/*.c $(srcdir)/*/*.c $(srcdir)/*/*/*.c))
BASE_OBJS := $(patsubst $(srcdir)/%.c,%.o,$(wildcard $(srcdir)/*.c $(srcdir)/qdma_access/*.c $(srcdir)/hwmon/*.c))
onic-objs = $(BASE_OBJS)
#ccflags-y = -O3 -Wall -Werror -I$(srcdir)/qdma_access -I$(srcdir)/hwmon -I$(srcdir)
ccflags-y = -O3 -Wall -I$(srcdir)/qdma_access -I$(srcdir)/hwmon -I$(srcdir) -DONIC_MAX_QUEUES=$(ONIC_MAX_QUEUES)

KDIR ?= /lib/modules/$(shell uname -r)/build


all:
	$(COMPILEDB) make -C $(KDIR) M=$(PWD) modules
	
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f *.o.ur-safe
	rm -f ./qdma_access/*.o.ur-safe
	make -C $(srcdir)/utility clean

install:
	rm -f /lib/modules/$(KERNEL_VERS)/onic.ko
	cp onic.ko /lib/modules/$(KERNEL_VERS)
	depmod

uninstall:
	rm -f /lib/modules/$(KERNEL_VERS)/onic.ko
	depmod

build_utility: 
	make -C $(srcdir)/utility
kprobe: 
	#clang -O2 -g -target bpf -I/usr/src/linux-hwe-6.5-headers-6.5.0-18/arch/x86/include/uapi -c kprobe.bpf.c -o kprobe.bpf.o
	make -C $(srcdir)/kprobe
up:
	sudo insmod onic.ko
	sudo ip link set enp65s0 up
down:
	sudo ip link set enp65s0 down
	sudo rmmod onic
