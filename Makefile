ifeq ($(COOKED),)
#Fedora specific installed kernel build tree
KERNEL_SOURCE ?= /usr/src/kernels/$(shell uname -r)

#lsm-sym.c:
#	grep

modules:
%:# lsm-syms.c
	make -C $(KERNEL_SOURCE) M=$(PWD) COOKED=1 $@
endif

#untested
#obj-m += livepatch-cooked.o
obj-m += lsm-cooked.o
