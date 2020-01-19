KERNELDIR := /home/wzh/wzh_learning/nxp_uboot\&linux/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)

obj-m := sdio_uart.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	
	echo moduels 
	echo M

	cp sdio_uart.ko /home/wzh/wzh_learning/nfs/buildrootfs/root

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean