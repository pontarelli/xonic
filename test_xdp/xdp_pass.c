#include <linux/bpf.h>
#include <linux/if_ether.h>

#ifndef __section
# define __section(NAME)                  \
		__attribute__((section(NAME), used))
#endif

__section("xdp_prog")
int xdp_pass(struct xdp_md *ctx)
{
	void* data_end = (void*)(long)ctx->data_end;
	void* data = (void*)(long)ctx->data;
	struct ethhdr* eth = data;

	if (data + sizeof(*eth) > data_end) {
		return XDP_PASS;
	}

	return XDP_PASS;
	//return XDP_DROP;
}

char __license[] __section("license") = "GPL";
