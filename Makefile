ifeq ($(COOKED),)
DISTRO=$(shell awk -F= '/^NAME=/ { print $$2 }' /etc/os-release | tr -d \" )
ifeq ($(DISTRO),Fedora)
# Fedora specific installed kernel build tree
# Fedora disables CONFIG_LIVEPATCH, so it shouldn't compile anyway
KERNEL_SOURCE ?= /usr/src/kernels/$(shell uname -r)
endif
$(info distro: $(DISTRO))
ifeq ($(DISTRO),Ubuntu)
KERNEL_SOURCE ?= /usr/src/linux-headers-$(shell uname -r)
endif

modules:
%:
	make -C $(KERNEL_SOURCE) M=$(PWD) COOKED=1 $@
endif

# Works on Ubuntu. Disable to unload:
# echo 0 | sudo tee -a /sys/kernel/livepatch/livepatch_cooked/enabled
obj-m += livepatch-cooked.o
# not working
#obj-m += lsm-cooked.o
