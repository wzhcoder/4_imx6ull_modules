KERNELDIR := /home/wzh/wzh_learning/nxp_uboot\&linux/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)

obj-m   := pin_gpio_led.o
obj.ko	:= pin_gpio_led.ko

app.c   := pin_gpio_led_app.c
app     := pin_gpio_led_app



build: kernel_modules app move

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	
	echo moduels 
	echo M

	
app:
	arm-linux-gnueabihf-gcc -o $(app) $(app.c)
move:
	cp $(obj.ko) $(app) /home/wzh/wzh_learning/nfs/buildrootfs/lib/modules/4.1.15/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
	rm $(app)