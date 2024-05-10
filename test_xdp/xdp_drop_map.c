#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct bpf_map_def SEC("maps") my_map = {
	.type = BPF_MAP_TYPE_ARRAY,
	.key_size = sizeof(int),
	.value_size = sizeof(__u32),
	.max_entries = 4,
};


#ifndef __section
# define __section(NAME)                  \
	__attribute__((section(NAME), used))
#endif

__section("xdp")
int xdp_drop(struct xdp_md *ctx)
{
	__u32 key = XDP_DROP; /* XDP_DROP = 1 */
	__u32* value = bpf_map_lookup_elem(&my_map, &key);
	if (value)
		__sync_fetch_and_add(value, 1);

	return XDP_DROP;
}

char __license[] __section("license") = "GPL";
