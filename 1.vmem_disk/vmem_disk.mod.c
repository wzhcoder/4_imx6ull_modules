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
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x18e90764, __VMLINUX_SYMBOL_STR(blk_cleanup_queue) },
	{ 0xf47339a5, __VMLINUX_SYMBOL_STR(put_disk) },
	{ 0x97726489, __VMLINUX_SYMBOL_STR(del_gendisk) },
	{ 0x208614a6, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xb5a459dc, __VMLINUX_SYMBOL_STR(unregister_blkdev) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x967825dd, __VMLINUX_SYMBOL_STR(add_disk) },
	{ 0xb81960ca, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xba02da45, __VMLINUX_SYMBOL_STR(alloc_disk) },
	{ 0xaf8b62f6, __VMLINUX_SYMBOL_STR(blk_queue_logical_block_size) },
	{ 0x715d0819, __VMLINUX_SYMBOL_STR(blk_init_queue) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xcca6874c, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x71a50dbc, __VMLINUX_SYMBOL_STR(register_blkdev) },
	{ 0xb5ac343b, __VMLINUX_SYMBOL_STR(bio_endio) },
	{ 0xfb6f1259, __VMLINUX_SYMBOL_STR(__blk_end_request_all) },
	{ 0xdf8dd48d, __VMLINUX_SYMBOL_STR(blk_start_request) },
	{ 0x74e8f4, __VMLINUX_SYMBOL_STR(blk_peek_request) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0xed93f29e, __VMLINUX_SYMBOL_STR(__kunmap_atomic) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xa97575ce, __VMLINUX_SYMBOL_STR(kmap_atomic) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "608989E20C488099F47093D");
