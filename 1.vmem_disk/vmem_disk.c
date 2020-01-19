/*
 * Sample disk driver, from the beginning.
 */


/* 说明
vmem_disk是一种模拟磁盘，数据实际上存在RAM中，通过vmalloc分配
出来的内存空间模拟磁盘，以块设备的方式访问这个区域

如何测试 
加载驱动模块后，会在/dev下生成4个块设备节点vmem_disk*
执行 mke2fs  /dev/块设备节点名（任选一个）创建ext2格式文件系统
然后可以挂载了 
mount /dev/vmem_diska /mnt */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>        
#include <linux/fs.h>
#include <linux/hdreg.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/bio.h>


#define HARDSECT_SIZE 512	//扇区的大小设为512字节
#define NSECTORS 1024  		//设置1024个扇区
#define NDEVICES 4			//块设备的数量

enum {
	VMEMD_QUEUE  = 0, /* Use request_queue */
	VMEMD_NOQUEUE = 1, /* Use make_request */
};

static int vmem_disk_major;
static int request_mode = VMEMD_QUEUE;



#define VMEM_DISK_MINORS    16
#define KERNEL_SECTOR_SIZE  512

struct vmem_disk_dev {
	int size;                       /* Device size in sectors */
	u8 *data;                       /* The data array */
	spinlock_t lock;                /* For mutual exclusion */
	struct request_queue *queue;    /* The device request queue */
	struct gendisk *gd;             /* The gendisk structure */
};

static struct vmem_disk_dev *devices;

/*
 * Handle an I/O request.
 */
/* 实现真正的硬件IO操作，对本例而言就是memcpy ,在实际环境中使用io函数操作*/
/* 驱动从偏移扇区sector开始写入或读取nsect个扇区 */
/* 块设备dev 从 buffer地址处向（sector x512 + 存储器的起始地址）处 写入 nsect 个扇区 
或者 从（sector x512 + 存储器的起始地址）处 读取 nsect 个扇区到 buffer中*/
static void vmem_disk_transfer(struct vmem_disk_dev *dev, unsigned long sector,
		unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector*KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;

	if ((offset + nbytes) > dev->size) {
		printk (KERN_NOTICE "写入/读取数据超出存储器的范围 (%ld %ld)\n", offset, nbytes);
		return;
	}
	if (write)
		memcpy(dev->data + offset, buffer, nbytes);//data存放块设备的起始地址
		/* 从buffer地址处向（起始地址+偏移地址）地址处 写入 nbytes个数据 */
	else
		memcpy(buffer, dev->data + offset, nbytes);
}

/*
 * Transfer a single BIO.
 */
/* 完成一个与bio对应的硬件操作 */
static int vmem_disk_xfer_bio(struct vmem_disk_dev *dev, struct bio *bio)
{
		/* bio_vec指定 页中的一块连续的区域 ，在bio中描述的就是一个页中的一个segment */
        struct bio_vec bvec;
		/* bvec_iter 记录当前bvec被处理的情况，用于遍历bio */
        struct bvec_iter iter;
        sector_t sector = bio->bi_iter.bi_sector;//偏移扇区

	/* 遍历bio的每一个segment ，即每一个bvec */
	bio_for_each_segment(bvec, bio, iter) {
		char *buffer = __bio_kmap_atomic(bio, iter);
		vmem_disk_transfer(dev, sector, bio_cur_bytes(bio) >> 9,
			buffer, bio_data_dir(bio) == WRITE);
			/*  bio_data_dir(bio) 返回IO操作的方向，是读还是写*/
		sector += bio_cur_bytes(bio) >> 9;
		/* bio_cur_bytes(bio) 返回当前段中需要参数的字节数 */
		__bio_kunmap_atomic(buffer);
	}
	return 0;
}

/*
 * The request_queue version.
 */
/*作用是做出与request对应的块设备IO操作  */
static void vmem_disk_request(struct request_queue *q)
{
	struct request *req;
	struct bio *bio;

	while ((req = blk_peek_request(q)) != NULL) {
		struct vmem_disk_dev *dev = req->rq_disk->private_data;
		if (req->cmd_type != REQ_TYPE_FS) {
			printk (KERN_NOTICE "Skip non-fs request\n");
			blk_start_request(req);
			__blk_end_request_all(req, -EIO);
			continue;
		}

		blk_start_request(req);/* 开启请求 */
		__rq_for_each_bio(bio, req)		/* 遍历每一个bio */
			vmem_disk_xfer_bio(dev, bio);
		__blk_end_request_all(req, 0);/* 关闭请求 */
	}
}


/*
 * The direct make request version.
 */
static void vmem_disk_make_request(struct request_queue *q, struct bio *bio)
{
	struct vmem_disk_dev *dev = q->queuedata;
	int status;

	status = vmem_disk_xfer_bio(dev, bio);
	bio_endio(bio, status);
}

static int vmem_disk_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	long size;
	struct vmem_disk_dev *dev = bdev->bd_disk->private_data;

	size = dev->size*(HARDSECT_SIZE/KERNEL_SECTOR_SIZE);
	geo->cylinders = (size & ~0x3f) >> 6; //磁柱（磁道）的个数
	geo->heads = 4;						 //磁头的个数，即盘面数
	geo->sectors = 16;					// 扇区的个数
	geo->start = 4;						//从哪个磁道开始
					//总容量为 cylinders x heads x sectors x 512  （字节）

	return 0;
}

/*
 * The device operations structure.
 */
static struct block_device_operations vmem_disk_ops = {
	.getgeo          = vmem_disk_getgeo,  
	/* 获得驱动器信息，struct hd_geometry 包含磁头、扇区等信息 */
};

static void setup_device(struct vmem_disk_dev *dev, int which)
{
	memset (dev, 0, sizeof (struct vmem_disk_dev));
	/* 把dev当前位置后面的 sizeof (struct vmem_disk_dev) 个字节全部赋值为0 
	这种方法速度快*/
	dev->size = NSECTORS*HARDSECT_SIZE; //设置一个块设备的总的大小，单位字节
	dev->data = vmalloc(dev->size);	//一个虚拟块设备在ram中的起始地址
	if (dev->data == NULL) {
		printk (KERN_NOTICE "vmalloc failure.\n");
		return;
	}
	spin_lock_init(&dev->lock);

	/*
	 * The I/O queue, depending on whether we are using our own
	 * make_request function or not.
	 */
	switch (request_mode) {
	case VMEMD_NOQUEUE://不使用队列请求，一般用在RAMDISK 或者ZRAM
		dev->queue = blk_alloc_queue(GFP_KERNEL);
		if (dev->queue == NULL)
			goto out_vfree;
		blk_queue_make_request(dev->queue, vmem_disk_make_request);
		break;
	default:
		printk(KERN_NOTICE "Bad request mode %d, using simple\n", request_mode);
	case VMEMD_QUEUE: //使用队列请求 ，用在机械硬盘
		dev->queue = blk_init_queue(vmem_disk_request, &dev->lock);
		/* 初始化请求队列，第一个参数是请求处理函数 */
		if (dev->queue == NULL)
			goto out_vfree;
		break;
	}
	blk_queue_logical_block_size(dev->queue, HARDSECT_SIZE);
	/* 告知内核 块设备扇区的大小 */
	dev->queue->queuedata = dev;

	dev->gd = alloc_disk(VMEM_DISK_MINORS);/* 分配块设备的分区数量 */
	if (!dev->gd) {
		printk (KERN_NOTICE "alloc_disk failure\n");
		goto out_vfree;
	}
	dev->gd->major = vmem_disk_major;
	dev->gd->first_minor = which*VMEM_DISK_MINORS;
	dev->gd->fops = &vmem_disk_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	snprintf (dev->gd->disk_name, 32, "vmem_disk%c", which + 'a');
	set_capacity(dev->gd, NSECTORS*(HARDSECT_SIZE/KERNEL_SECTOR_SIZE));
	/* 设置磁盘的容量，以扇区为单位，扇区分为物理扇区（512的整数倍个字节）
		和内核扇区（512字节）， */
	add_disk(dev->gd);/* 激活块设备 */
	return;

out_vfree:
	if (dev->data)
		vfree(dev->data);
}


static int __init vmem_disk_init(void)
{
	int i;

	vmem_disk_major = register_blkdev(vmem_disk_major, "vmem_disk");
	/* 第一个输入参数为0，内核会自动分配一个主设备号 ，第2个是设备名，返回值是分配的主设备号*/
	if (vmem_disk_major <= 0) {
		printk(KERN_WARNING "vmem_disk:分配不到主设备号\n");
		return -EBUSY;
	}

	devices = kmalloc(NDEVICES*sizeof (struct vmem_disk_dev), GFP_KERNEL);
	/* 乘 NDEVICES 是为了创建NDEVICES 个块设备*/
	/* kmalloc 和vmalloc 是用于内核或者驱动中的，不能用malloc
	kmalloc申请物理上连续的地址 可以用在DMA中，释放使用kmalloc
	而 vmalloc申请线性上连续的地址，物理空间不一定连续，不能用在DMA中
	*/
	if (!devices)
		goto out_unregister;
	for (i = 0; i < NDEVICES; i++)
		setup_device(devices + i, i);

	return 0;

out_unregister:
	unregister_blkdev(vmem_disk_major, "sbd");
	return -ENOMEM;
}

static void vmem_disk_exit(void)
{
	int i;

	for (i = 0; i < NDEVICES; i++) {
		struct vmem_disk_dev *dev = devices + i;

		if (dev->gd) {
			del_gendisk(dev->gd);
			put_disk(dev->gd); /* 删除块设备的引用计数 */
		}
		if (dev->queue) {
			if (request_mode == VMEMD_NOQUEUE)
				kobject_put (&dev->queue->kobj);
			else
				blk_cleanup_queue(dev->queue);
		}
		if (dev->data)
			vfree(dev->data);
	}
	unregister_blkdev(vmem_disk_major, "vmem_disk");
	kfree(devices);/* 释放创造的虚拟内存块 */
}


module_init(vmem_disk_init);
module_exit(vmem_disk_exit);
MODULE_AUTHOR("wzh");
MODULE_LICENSE("GPLv2");
