#define __x86_64__
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include <linux/ptrace.h>



SEC( "kprobe/__dev_ethtool" )
int kprobe(struct pt_regs *ctx) {
	bpf_printk("kprobe_ethtool\n");
	__u32 ethcmd = (__u32)PT_REGS_PARM4(ctx);
	bpf_printk("ethcmd: %u\n", ethcmd);
	return 0;
}
char _license[] SEC("license") = "GPL";
