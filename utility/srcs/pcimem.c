#include "tool.h"

int	pcimem(t_options *o)
{
	int				fd;
	off_t			target_base;
	unsigned int	map_size;
	void			*map_base;
	void			*virt_addr;
	uint64_t		read_result;


	fd = open(RESOURCE, O_RDWR | O_SYNC);
	if (fd < 0) {
		perror("Failed to open resource");
		return (1);
	}

	map_size = 4096;
	target_base = o->target & ~(sysconf(_SC_PAGE_SIZE) - 1);
	if (o->target - target_base > map_size)
		map_size = o->target - target_base;

	map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target_base);
	if (map_base == MAP_FAILED) {
		perror("Failed to map memory");
		goto ERROR;
	}

	virt_addr = map_base + o->target - target_base;
	if (o->write >= 0)
		*((uint32_t *) virt_addr) = o->write;
	read_result = *((uint32_t *) virt_addr);

	printf("0x%04X: 0x%08lX (%ld)\n", (int)(o->target), read_result, read_result);

	if (munmap(map_base, map_size) == -1) {
		perror("Failed to unmap memory");
		goto ERROR;
	}

	return (0);
ERROR:
	close(fd);
	return (1);
}

int pci_interact(t_options *o)
{
	if (getuid() != 0) {
		printf("Permission Denied (try sudo)\n");
		return (1);
	}

	if (pcimem(o))
		return (1);
	
	return (0);
}
