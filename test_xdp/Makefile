NO_COLOR=\e[0m
OK_COLOR=\e[32;01m
ERROR_COLOR=\e[31;01m
WARN_COLOR=\e[33;01m

help:
	@echo -e '$(OK_COLOR)'
	@echo -e 'Usage: $(NO_COLOR)'
	@echo -e 'make help: display this message'
	@echo -e 'make all: build the xdp test'
	@echo -e 'make install-gen: load the xdp code with generic driver'
	@echo -e 'make install: load the xdp code'

all:
	clang -O2 -g -Wall -target bpf -c xdp_drop.c -o xdp_drop.o
	clang -O2 -g -Wall -target bpf -c xdp_pass.c -o xdp_pass.o
	clang -O2 -g -Wall -target bpf -c xdp_tx.c -o xdp_tx.o
	clang -O2 -g -Wall -target bpf -c xdp_drop_map.c -o xdp_drop_map.o
	clang -O2 -g -Wall -target bpf -c xdp_tx_iptunnel.c -o xdp_tx_iptunnel.o
	clang -O2 -g -Wall -target bpf -c xdp_fw.c -o xdp_fw.o
	#run with LD_LIBRARY_PATH=/home/sal/libbpf/src/ ./xdp_tx_iptunnel_user
	clang -L/home/sal/libbpf/src/ -O2 -g -Wall xdp_tx_iptunnel_user.c  -lbpf -o xdp_tx_iptunnel_user
install-gen:
	sudo ip link set enp1s0 xdpgeneric obj xdp_drop.o sec xdp_prog
install:
	sudo ip link set enp1s0 xdp obj xdp_drop.o sec xdp_prog

