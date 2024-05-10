# include "tool.h"

void	dbg_options(t_options *o)
{
	printf("Device: %s\n", o->device);
	printf("Command: %d\n", o->command);
	printf("Target: 0x%04lX\n", o->target);
	if (o->write >= 0)
		printf("Write: 0x%08llX\n", o->write);
	else
		printf("Write: %lld\n", o->write);
	printf("\n");
}
