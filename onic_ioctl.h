#include "onic.h"
#include <linux/ioctl.h>
#include <linux/netdevice.h>

#define ONIC_IOCTL_MAGIC 'o'
#define ONIC_IOCTL_ENABLE  _IO(ONIC_IOCTL_MAGIC, 0)
#define ONIC_IOCTL_DISABLE _IO(ONIC_IOCTL_MAGIC, 1)

int onic_ioctl_enable(struct net_device *netdev);

ssize_t onic_fs_read(struct file *, char __user *, size_t, loff_t *);
ssize_t onic_fs_write(struct file *, const char __user *, size_t, loff_t *);
int onic_fs_open(struct inode *, struct file *);
long onic_fs_ioctl(struct file *, unsigned int, unsigned long);
int onic_fs_release(struct inode *, struct file *);
int onic_create_chrdev(struct onic_device_data*);
int onic_remove_chrdev(struct onic_device_data*);
