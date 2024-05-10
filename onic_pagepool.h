#ifndef ONIC_PAGEPOOL_H
#define ONIC_PAGEPOOL_H
#include <linux/types.h>
#define PAGEPOOL_SIZE 1024
#define PAGEPOOL_REFILL_THRESHOLD 128

typedef struct pagepool_entry {
	struct page* page;
} pagepool_entry_t;

int pagepool_init(void);
struct page* pagepool_get_page(void);
void pagepool_exit(void);
int pagepool_refill(void);
#endif
