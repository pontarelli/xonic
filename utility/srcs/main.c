#include "tool.h"

int	command_device(t_options *o, int fd)
{
	int	err;

	switch (o->command) {
		case 0:
			err = ioctl(fd, ONIC_IOCTL_DISABLE);
			if (err)
				perror("Failed to disable device");
			return (err);
		case 1:
			err = ioctl(fd, ONIC_IOCTL_ENABLE);
			if (err)
				perror("Failed to enable device");
			return (err);
		default:
			printf("Invalid command\n");
			return (1);
	}
	return (0);
}

int execute_command(t_options *o)
{
	int	fd;
	int	err;

	if (o->device == NULL) {
		printf("No device specified\n");
		return (1);
	}
	if (o->command < 0) {
		printf("No command specified\n");
		return (1);
	}

	fd = open(o->device, O_RDWR);
	if (fd < 0) {
		perror("Failed to open device");
		return (1);
	}

	err = command_device(o, fd);

	close(fd);
	return (err);
}

int	main(int argc, char **argv)
{
	t_options	o;
	int			err;

	if (arg_parse(&o, argc, argv))
		return (1);
	#if DEBUG
		dbg_options(&o);
	#endif

	if (o.target >= 0)
		err = pci_interact(&o);
	else
		err = execute_command(&o);

	return (err);
}
