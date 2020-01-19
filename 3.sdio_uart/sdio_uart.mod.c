#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfa985410, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc7afa9f7, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xd6060075, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x2000cf79, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x6076c1c8, __VMLINUX_SYMBOL_STR(sdio_unregister_driver) },
	{ 0x67b27ec1, __VMLINUX_SYMBOL_STR(tty_std_termios) },
	{ 0xd1f260ce, __VMLINUX_SYMBOL_STR(put_tty_driver) },
	{ 0xa6a880c5, __VMLINUX_SYMBOL_STR(tty_unregister_driver) },
	{ 0x2391ae14, __VMLINUX_SYMBOL_STR(sdio_register_driver) },
	{ 0x8635699b, __VMLINUX_SYMBOL_STR(tty_register_driver) },
	{ 0x640914f2, __VMLINUX_SYMBOL_STR(tty_set_operations) },
	{ 0xf6a2358f, __VMLINUX_SYMBOL_STR(__tty_alloc_driver) },
	{ 0x208614a6, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x411b335a, __VMLINUX_SYMBOL_STR(tty_port_register_device) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xc068440e, __VMLINUX_SYMBOL_STR(__kfifo_alloc) },
	{ 0xf3bb59b5, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0xffa54252, __VMLINUX_SYMBOL_STR(tty_port_init) },
	{ 0xcca6874c, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x464971e9, __VMLINUX_SYMBOL_STR(tty_unregister_device) },
	{ 0x1216dbbe, __VMLINUX_SYMBOL_STR(sdio_release_irq) },
	{ 0x2d5d463f, __VMLINUX_SYMBOL_STR(tty_standard_install) },
	{ 0x5ccdf1ef, __VMLINUX_SYMBOL_STR(tty_port_open) },
	{ 0x919b74aa, __VMLINUX_SYMBOL_STR(tty_port_close) },
	{ 0xf23fcb99, __VMLINUX_SYMBOL_STR(__kfifo_in) },
	{ 0x805eae4c, __VMLINUX_SYMBOL_STR(tty_port_hangup) },
	{ 0xc6cbbc89, __VMLINUX_SYMBOL_STR(capable) },
	{ 0xdf7ab9ce, __VMLINUX_SYMBOL_STR(seq_putc) },
	{ 0xd21e7ed9, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x5fc56a46, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x9c0bd51f, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x23e615a2, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0x651dfc40, __VMLINUX_SYMBOL_STR(sdio_disable_func) },
	{ 0x49ebacbd, __VMLINUX_SYMBOL_STR(_clear_bit) },
	{ 0x205e3412, __VMLINUX_SYMBOL_STR(sdio_claim_irq) },
	{ 0xd8b9040e, __VMLINUX_SYMBOL_STR(sdio_enable_func) },
	{ 0x676bbc0f, __VMLINUX_SYMBOL_STR(_set_bit) },
	{ 0x51d559d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x13d0adf7, __VMLINUX_SYMBOL_STR(__kfifo_out) },
	{ 0x598542b2, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x61ff4083, __VMLINUX_SYMBOL_STR(tty_wakeup) },
	{ 0x81612abe, __VMLINUX_SYMBOL_STR(tty_port_tty_hangup) },
	{ 0xae0cb1fb, __VMLINUX_SYMBOL_STR(tty_flip_buffer_push) },
	{ 0xe86e264a, __VMLINUX_SYMBOL_STR(tty_insert_flip_string_flags) },
	{ 0x6d6c1ee1, __VMLINUX_SYMBOL_STR(tty_kref_put) },
	{ 0xb73926cd, __VMLINUX_SYMBOL_STR(tty_port_tty_get) },
	{ 0xd85cd67e, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x409873e3, __VMLINUX_SYMBOL_STR(tty_termios_baud_rate) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0x4a2fb659, __VMLINUX_SYMBOL_STR(sdio_writeb) },
	{ 0xd11b1f5c, __VMLINUX_SYMBOL_STR(sdio_release_host) },
	{ 0x7eb0b343, __VMLINUX_SYMBOL_STR(sdio_readb) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xdb760f52, __VMLINUX_SYMBOL_STR(__kfifo_free) },
	{ 0xb55f48e4, __VMLINUX_SYMBOL_STR(tty_port_put) },
	{ 0x49fcab7e, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xf922aa3e, __VMLINUX_SYMBOL_STR(sdio_claim_host) },
	{ 0x7267eeee, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("sdio:c01v*d*");
MODULE_ALIAS("sdio:c04v*d*");

MODULE_INFO(srcversion, "1D1DB96F5125988FDD64620");
