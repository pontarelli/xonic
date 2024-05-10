#include "tool.h"

void	init_options(t_options *o)
{
	o->device = NULL;
	o->command = -1;
	o->target = -1;
	o->write = -1;
}

int	set_device(t_options *o, char **args, size_t *i)
{
	if (!args[1])
		return (printf("Please specify a device\n"));
	if (o->device != NULL)
		return (printf("Cannot specify more than one device\n"));
	o->device = args[1];
	++(*i);
	return (0);
}

int	set_command(t_options *o, char **args, size_t *i)
{
	int	command;

	if (!args[1])
		return (printf("Please specify a command\n"));
	command = atoi(args[1]);
	if (command < 0 || command > 1)
		return (printf("Please specify a valid command (0/1)\n"));
	o->command = command;
	++(*i);
	return (0);
}

int	set_target(t_options *o, char **args, size_t *i)
{
	off_t	target;

	if (!args[1] || strchr(args[1], '-')) {
		o->target = TARGET;
		return (0);
	}

	target = strtoul(args[1], 0, 0);
	if (target < 0)
		return (printf("Please specify a valid target\n"));
	o->target = target;
	++(*i);
	return (0);
}

int	set_write(t_options *o, char **args, size_t *i)
{
	off_t	write_val;

	if (!args[1]) {
		return (printf("Please specify a write value\n"));
	}

	write_val = strtoul(args[1], 0, 0);
	if (write_val < 0)
		return (printf("Please specify a valid twrite value\n"));
	o->write = write_val;
	++(*i);
	return (0);
}

char	*flags_usage[] = {
	"-h", "prints the usage",
	"-d", "the device to interact with",
	"-c", "the command to disable/enable (0/1)",
	"-t", "the memory target to interact with",
	"-w", "the value to write to the memory target",
	NULL, NULL
};

int	(*flags[])(t_options *p, char **args, size_t *i) = {
	&set_device,
	&set_command,
	&set_target,
	&set_write,
	NULL
};

int	usage(char *name)
{
	size_t	i;

	printf("Usage: %s [flags]\n", name);
	i = 0;
	while (flags_usage[i])
	{
		printf("\t%s\t%s\n", flags_usage[i], flags_usage[i + 1]);
		i += 2;
	}
	printf("Examples:\n");
	printf("\t%s -d /dev/onic_device -c 0 (disables /dev/onic_device)\n", name);
	printf("\t%s -t 0x4000 -w 0xF (writes 0xF as word at 0x4000)\n", name);
	printf("\t%s -t (reads on default target: 0x8090)\n", name);
	return (1);
}

int	arg_parse(t_options *o, size_t argc, char **argv)
{
	size_t	i;
	size_t	j;

	if (argc < 2)
		return (usage(argv[0]));
	argc -= 1;
	argv = &argv[1];
	init_options(o);
	i = 0;
	while (i < argc)
	{
		j = 0;
		if (!strncmp(flags_usage[0], argv[i], 3))
			return (usage(argv[0]));
		while (flags_usage[(j + 1) * 2] != NULL)
		{
			if (!strncmp(flags_usage[(j + 1) * 2], argv[i],
				strlen(flags_usage[(j + 1) * 2]) + 1))
			{
				if (flags[j](o, &argv[i], &i))
					return (1);
				break ;
			}
			++j;
		}
		++i;
	}
	return (0);
}
