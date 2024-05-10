#ifndef TOOL_H
# define TOOL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/mman.h>

# define DEBUG 0

# define RESOURCE "/sys/bus/pci/devices/0000:01:00.0/resource2"
# define TARGET 0x0


# define ONIC_IOCTL_MAGIC 'o'
# define ONIC_IOCTL_ENABLE  _IO(ONIC_IOCTL_MAGIC, 0)
# define ONIC_IOCTL_DISABLE _IO(ONIC_IOCTL_MAGIC, 1)


typedef struct s_options
{
	char		*device;
	int			command;
	off_t		target;
	long long	write;
} t_options;


// debug.c
void	dbg_options(t_options *o);

// parse.c
int		arg_parse(t_options *o, size_t argc, char **argv);

// pcimem.c
int 	pci_interact(t_options *o);

#endif
