#include <linux/bpf.h>

#ifndef __section
# define __section(NAME)                  \
		__attribute__((section(NAME), used))
#endif

__section("xdp_prog")
int xdp_tx(struct xdp_md *ctx)
{
		return XDP_TX;
}

char __license[] __section("license") = "GPL";
