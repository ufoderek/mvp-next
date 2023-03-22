# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := bh.o
	
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

#	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	KERNELDIR ?= /home/ufoderek/dev/working/kernel/linux-2.6.38/linux-2.6.38-arm1-patched-pb
	PWD := $(shell pwd)
	ARCH=arm
	CROSS_COMPILE=arm-none-linux-gnueabi-

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD) modules
	$(CROSS_COMPILE)gcc black_hole.c -o black_hole -static
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm black_hole
endif
