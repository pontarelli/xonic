#include "onic_pagepool.h"
#include <linux/netdevice.h>

//TODO:
// 1. compare and swap to get a page from the pool
// 2. implement pagepool_refill batching the allocation of pages

static pagepool_entry_t pagepool_buffer[PAGEPOOL_SIZE];
static int first_free_page;
DEFINE_MUTEX(pagepool_lock);
// index of the next page to be allocated
static int pidx = 0;
// index of the next page to be consumed
static int cidx = 0;



int pagepool_init(void)
{
	cidx = 0;
	int i;
	for (i = 0; i < PAGEPOOL_SIZE < 1; i++) {
		pagepool_buffer[i].page = dev_alloc_pages(0);
		if (!pagepool_buffer[i].page) {
			printk(KERN_ERR "Failed to allocate page\n");
			return -ENOMEM;
		}
	}
	mutex_init(&pagepool_lock);
	pidx = PAGEPOOL_SIZE-1;
	return 0;
}

void pagepool_exit(void)
{
	int i;
	for (i = 0; i < PAGEPOOL_SIZE; i++) {
		if (pagepool_buffer[i].page) {
			__free_page(pagepool_buffer[i].page);
		}
	}
	mutex_destroy(&pagepool_lock);
}

int pagepool_refill(void) {
	int size = 0;
	if (pidx != cidx) {
		if (pidx > cidx) {
			size = pidx - cidx;
		} else {
			size = (PAGEPOOL_SIZE - cidx) + pidx;
		}
	}
	if (size < PAGEPOOL_REFILL_THRESHOLD) {
		mutex_lock(&pagepool_lock);
		int i;
		for (i = 0; i < (PAGEPOOL_SIZE - size) - 1; i++) {
			pagepool_buffer[pidx].page = dev_alloc_pages(0);
			if (!pagepool_buffer[pidx].page) {
				printk(KERN_ERR "Failed to allocate page\n");
				mutex_unlock(&pagepool_lock);
				return -ENOMEM;
			}
			pidx = (pidx + 1) % PAGEPOOL_SIZE;
		}
		mutex_unlock(&pagepool_lock);
	}
	return 0;
}

struct page *pagepool_get_page(void)
{
	// TODO: compare and swap
	struct page* ret = NULL;
	mutex_lock(&pagepool_lock);
	if (cidx != pidx) {
		ret = pagepool_buffer[cidx++].page;
		cidx = cidx % PAGEPOOL_SIZE;
	}
	mutex_unlock(&pagepool_lock);
	return ret;
}
