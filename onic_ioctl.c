#include "onic.h"
#include "onic_common.h"
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/version.h>
#include <onic_ioctl.h>
#include <onic_hardware.h>
#include <onic_register.h>



static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = onic_fs_read,
        .write          = onic_fs_write,
        .open           = onic_fs_open,
        .unlocked_ioctl = onic_fs_ioctl,
        .release        = onic_fs_release,
};

/*
** This function will be called when we open the Device file
*/
int onic_fs_open(struct inode *inode, struct file *file)
{
	struct onic_device_data *device_data = container_of(inode->i_cdev, struct onic_device_data, cd);
	struct onic_private *priv = device_data->priv;
	file->private_data = priv;
        pr_info("Device File Opened...!!!\n");
	// set file private data to the device
	
        return 0;
}
/*
** This function will be called when we close the Device file
*/
int onic_fs_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/

ssize_t onic_fs_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	struct onic_private *priv = filp->private_data;
        if (priv->metadata_en) 
		pr_info("Read Function (metadata enabled) \n");
	else
		pr_info("Read Function (metadata disabled) \n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
ssize_t onic_fs_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        return len;
}
long onic_fs_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct onic_private *priv = file->private_data;
	pr_info("IOCTL function\n");
	switch (cmd)
	{
	case ONIC_IOCTL_ENABLE:
		pr_info("IOCTL ENABLE\n");
		onic_write_reg(&(priv->hw), P250_CONTROL_REG, 1);
		onic_write_reg(&(priv->hw), QDMA_FUNC_CONTROL_REG(0), 3);
		priv->metadata_en = 1;
		break;
	case ONIC_IOCTL_DISABLE:
		pr_info("IOCTL DISABLE\n");
		onic_write_reg(&(priv->hw), P250_CONTROL_REG, 0);
		onic_write_reg(&(priv->hw), QDMA_FUNC_CONTROL_REG(0), 0);
		priv->metadata_en = 0;
		break;
	default:
		pr_info("IOCTL default\n");
		break;
	}
	return 0;
}


dev_t dev;
static struct class *dev_class;

int onic_create_chrdev(struct onic_device_data* device_data) {
	/*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "onic_dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&(device_data->cd),&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&(device_data->cd),dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,0,0)
        if(IS_ERR(dev_class = class_create("onic_class"))){
#else
        if(IS_ERR(dev_class = class_create(THIS_MODULE, "onic_class"))){
#endif
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev, NULL,"onic_device"))){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

int onic_remove_chrdev(struct onic_device_data* device_data) {
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&(device_data->cd));
	unregister_chrdev_region(dev, 1);
	pr_info("Device Driver Remove...Done!!!\n");
	return 0;
}
