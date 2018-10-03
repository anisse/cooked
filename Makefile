ifeq ($(COOKED),)
DISTRO=$(shell awk -F= '/^NAME=/ { print $$2 }' /etc/os-release | tr -d \" )
ifeq ($(DISTRO),Fedora)
#Fedora specific installed kernel build tree
KERNEL_SOURCE ?= /usr/src/kernels/$(shell uname -r)
endif
$(info distro: $(DISTRO))
ifeq ($(DISTRO),Ubuntu)
KERNEL_SOURCE ?= /usr/src/linux-headers-$(shell uname -r)
endif


#lsm-sym.c:
#	grep

modules:
%:# lsm-syms.c
	make -C $(KERNEL_SOURCE) M=$(PWD) COOKED=1 $@
endif

#untested
obj-m += livepatch-cooked.o
# not working
#obj-m += lsm-cooked.o
