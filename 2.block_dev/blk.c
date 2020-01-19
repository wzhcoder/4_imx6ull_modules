/* 在struct block_device_operations 中
int (*media_changed) (struct gendisk *); 介质改变函数
检查块设备是否改变，如果改变，返回一个非0值，否则返回0值。 仅对能移动的块设备有效

int (*revalidate_disk) (struct gendisk *); 使介质有效函数

int (*getgeo)(struct block_device *, struct hd_geometry *);获得驱动器信息

struct module *owner;  模块指针，几乎所有驱动，都初始化为THIS_MODULE,表示当前运行模块
*/

// static int __init xxx_blkdev_init(void)
// {
// 	int ret;
// 	static struct gendisk 		*xxx_disk;
// 	static struct request_queue *xxx_queue;

// 	xxx_disk = alloc_disk(1);  
// 	/* 分配gendisk，alloc_disk ( int minors)
// 	minors是这个磁盘的分区数量，值为0的话，代表整个设备 */
// 	if(!xxx_disk){
// 		ret = -ENOMEM;
// 		goto err_alloc_disk;
// 	}

// 	if(register_blkdev(xxx_MAJOR, "xxx") < 0){
// 		/* 注册块设备 ,xxx是设备号的名字，在/proc/devices中显示*/
// 		ret = -EBUSY;
// 		goto err_alloc_disk;
// 	}

// 	xxx_queue = blk_init_queue(xxx_request, NULL);
// 	/* 块设备队列初始化 */
// 	if(!xxx_queue){
// 		ret = -ENOMEM;
// 		goto err_init_queue;
// 	}

// 	strcpy(xxx_disk->disk_name, XXX_DISKNAME);
// 	xxx_disk->major = xxx_MAJOR;  //主设备号
// 	xxx_disk->first_minor = 0;   //次设备号
// 	xxx_disk->fops = &xxx_fops;
// 	xxx_disk->queue = xxx_queue;
// 	set_capacity(xxx_disk, xxx_BYTES >> 9);
// /* 	设置磁盘的容量，以扇区为单位，扇区分为物理扇区（512的整数倍个字节）和内核扇区（512字节），
// 	xxx_BYTES>>9 实际上就是将设备的字节容量除以512后得到的内核扇区数 */
// 	add_disk(xxx_disk);
// 	/* 激活硬盘设备 */

// 	return 0;

// err_init_queue:
// 	unregister_blkdev(xxx_MAJOR, "xxx");
// err_alloc_disk:
// 	put_disk(xxx_disk);
// 	return ret;
// }


// static void __exit xxx_blkdev_exit(void)
// {
// 	del_gendisk(xxx_disk);
// 	put_disk(xxx_disk);
// 	/* 在调用del_gendisk后，要引用put_disk减少gendisk的引用计数 
// 	因为在add_disk中增加了gendisk的引用计数*/
// 	blk_cleanup_queue(xxx_queue);
// 	unregister_blkdev(xxx_major, "xxx");
// }




/* 不使用请求队列的块设备驱动程序
该程序在内存中开辟了一个8M的内存空间来模拟实际的物理块设备 */
/* 该驱动运行在alpha板上，该板上的系统不支持udev文件系统，使用下面程序无法自动
在 /dev/ 下创建 Virtual_blkdev块设备文件，需要敲入 
mknod /dev/Virtual_blkdev  b  72  0       表示创建主设备号为72，次设备号为0*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/timer.h>
#include <linux/types.h>	/* size_t */
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/hdreg.h>	/* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>	/* invalidate_bdev */
#include <linux/bio.h>
#include <linux/elevator.h>

#define VIRTUAL_BLKDEV_DEVICEMAJOR  COMPAQ_SMART2_MAJOR
#define VIRTUAL_BLKDEV_DISKNAME 	"Virtual_blkdev"


#define VIRTUAL_BLKDEV_DEVICEMAJOR  COMPAQ_SMART2_MAJOR /*主设备号*/
#define VIRTUAL_BLKDEV_DISKNAME 	"Virtual_blkdev"/*设备名*/
#define VIRTUAL_BLKDEV_BYTES 		(8*1024*1024)/*设备的大小为8M*/
static struct request_queue 		*Virtual_blkdev_queue;/*请求队列指针*/
static struct gendisk 				*Virtual_blkdev_disk;/*通用磁盘*/
unsigned char 	Virtual_blkdev_data[VIRTUAL_BLKDEV_BYTES];/*8M的线性静态内存空间*/




static void Virtual_blkdev_do_request(struct request_queue *q)
{
	/* struct request 在 <linux/blkdev.h>中 */
	struct request *req;/* 指向要处理请求的指针 */
	while (elv_next_request(q) != NULL) /* 链表队列中查找 */
	{	/* 如果请求要写入的数据大于Virtual_blkdev设备的容量就退出 */
		if ((（req->__sector + req->current_nr_sectors)<<9） > VIRTUAL_BLKDEV_BYTES) 
		{
		printk(KERN_ERR VIRTUAL_BLKDEV_DISKNAME": bad request: block=%llu, count=%u\n",
			(unsigned long long)req->__sector,
			req->current_nr_sectors);
		end_request(req, 0); //为0表示，请求失败
		continue;
	    }/*endif*/
		switch (rq_data_dir(req)) /* 判断一个请求是写请求还是读请求 */
		{
			case READ:
				memcpy(req->buffer,Virtual_blkdev_data + (req->__sector << 9),
					req->current_nr_sectors << 9);
				end_request(req, 1); //为1，表示请求成功
				break;
			case WRITE:
				memcpy(Virtual_blkdev_data + (req->__sector << 9),
					req->buffer, req->current_nr_sectors << 9);
				end_request(req, 1);
				break;
			default:
				/* No default because rq_data_dir(req) is 1 bit */
				break;
		}
	}/*endwhile*/
}

struct block_device_operations Virtual_blkdev_fops =
{
	.owner = THIS_MODULE,
};

static int __init Virtual_blkdev_init(void)
{
	int ret;
	Virtual_blkdev_disk = alloc_disk(1);
	if (!Virtual_blkdev_disk) 
	{
		ret = -ENOMEM;
		goto err_alloc_disk;
	}
	Virtual_blkdev_queue = blk_init_queue(Virtual_blkdev_do_request, NULL);
	if (!Virtual_blkdev_queue) 
	{
		ret = -ENOMEM;
		goto err_init_queue;
	}

	strcpy(Virtual_blkdev_disk->disk_name, VIRTUAL_BLKDEV_DISKNAME);
	Virtual_blkdev_disk->major = VIRTUAL_BLKDEV_DEVICEMAJOR;
	Virtual_blkdev_disk->first_minor = 0;
	Virtual_blkdev_disk->fops = &Virtual_blkdev_fops;
	Virtual_blkdev_disk->queue = Virtual_blkdev_queue;
	set_capacity(Virtual_blkdev_disk, VIRTUAL_BLKDEV_BYTES>>9);
	add_disk(Virtual_blkdev_disk);

	printk("I'm in !\r\n");
	return 0;
	err_init_queue:
		put_disk(Virtual_blkdev_disk);
	err_alloc_disk:
		return ret;
}
static void __exit Virtual_blkdev_exit(void)
{
	del_gendisk(Virtual_blkdev_disk);
	put_disk(Virtual_blkdev_disk);
	blk_cleanup_queue(Virtual_blkdev_queue);
	printk("I'm out !\r\n");
}
module_init(Virtual_blkdev_init);
module_exit(Virtual_blkdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("wzh");
