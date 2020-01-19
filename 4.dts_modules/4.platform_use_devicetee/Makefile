KERNELDIR := /home/wzh/wzh_learning/nxp_uboot\&linux/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)

obj-m   := dtsplatform.o
obj.ko	:= dtsplatform.ko




build: kernel_modules  move

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	
	echo moduels 
	echo M

move:
	cp $(obj.ko) /home/wzh/wzh_learning/nfs/buildrootfs/lib/modules/4.1.15/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
