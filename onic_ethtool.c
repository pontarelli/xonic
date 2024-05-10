/*
 * Copyright (c) 2020 Xilinx, Inc.
 * All rights reserved.
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 */
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>

#include "onic.h"
#include "onic_register.h"

extern const char onic_drv_name[];
extern const char onic_drv_ver[];

enum {NETDEV_STATS, RING_STATS, ONIC_STATS};

struct onic_stats {
    char stat_string[ETH_GSTRING_LEN];
    int type;
    int sizeof_stat;
    int stat0_offset;
    int stat1_offset;
};

#define xstr(s) str(s)
#define str(s) #s

#define _STAT(_name, _stat0, _stat1) { \
	.stat_string = _name, \
	.type = ONIC_STATS, \
	.sizeof_stat = sizeof(u32), \
	.stat0_offset = _stat0, \
	.stat1_offset = _stat1, \
}
#define _RING_STAT(_name) { \
	.stat_string = str(_name), \
	.type = RING_STATS, \
	.sizeof_stat = sizeof(u32), \
      .stat0_offset = offsetof(struct onic_ring, _name), \
	.stat1_offset = 0, \
}

#define _NETDEV_STAT(_name) { \
	.stat_string = str(_name), \
	.type = NETDEV_STATS, \
	.sizeof_stat = sizeof(u32), \
      .stat0_offset = offsetof(struct onic_private, _name), \
      .stat1_offset = 0, \
}

static const struct onic_stats onic_gstrings_stats[] = {
    _RING_STAT(xdp_packets),
    _RING_STAT(xdp_pass),
    _RING_STAT(xdp_tx),
    _RING_STAT(xdp_drop),
    _NETDEV_STAT(netdev_stats.rx_packets),
    _STAT("debug_reg_pkt_in",
          0x100004,
          0x100104),
    _STAT("debug_reg_pkt_out",
          0x100008,
          0x100108),
    _STAT("debug_reg_frame_in",
          0x100014,
          0x100114),
    _STAT("debug_reg_frame_out",
          0x100018,
          0x100118),
    _STAT("debug_pkt_count",
          0x100020,
          0x100120),
    _STAT("debug_rd_ptr",
          0x100024,
          0x100124),
    _STAT("debug_wr_ptr",
          0x100028,
          0x100128),
    _STAT("debug_first",
          0x100030,
          0x100130),
    _STAT("debug_second",
          0x100034,
          0x100134),
    _STAT("stat_tx_total_pkts",
          CMAC_OFFSET_STAT_TX_TOTAL_PKTS(0),
          CMAC_OFFSET_STAT_TX_TOTAL_PKTS(1)),
    _STAT("stat_tx_total_good_pkts",
          CMAC_OFFSET_STAT_TX_TOTAL_GOOD_PKTS(0),
          CMAC_OFFSET_STAT_TX_TOTAL_GOOD_PKTS(1)),
    _STAT("stat_tx_total_bytes",
          CMAC_OFFSET_STAT_TX_TOTAL_BYTES(0),
          CMAC_OFFSET_STAT_TX_TOTAL_BYTES(1)),
    _STAT("stat_tx_total_good_bytes",
          CMAC_OFFSET_STAT_TX_TOTAL_GOOD_BYTES(0),
          CMAC_OFFSET_STAT_TX_TOTAL_GOOD_BYTES(1)),
    _STAT("stat_tx_pkt_64_bytes",
          CMAC_OFFSET_STAT_TX_PKT_64_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_64_BYTES(1)),
    _STAT("stat_tx_pkt_65_127_bytes",
          CMAC_OFFSET_STAT_TX_PKT_65_127_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_65_127_BYTES(1)),
    _STAT("stat_tx_pkt_128_255_bytes",
          CMAC_OFFSET_STAT_TX_PKT_128_255_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_128_255_BYTES(1)),
    _STAT("stat_tx_pkt_256_511_bytes",
          CMAC_OFFSET_STAT_TX_PKT_256_511_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_256_511_BYTES(1)),
    _STAT("stat_tx_pkt_512_1023_bytes",
          CMAC_OFFSET_STAT_TX_PKT_512_1023_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_512_1023_BYTES(1)),
    _STAT("stat_tx_pkt_1024_1518_bytes",
          CMAC_OFFSET_STAT_TX_PKT_1024_1518_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_1024_1518_BYTES(1)),
    _STAT("stat_tx_pkt_1519_1522_bytes",
          CMAC_OFFSET_STAT_TX_PKT_1519_1522_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_1519_1522_BYTES(1)),
    _STAT("stat_tx_pkt_1523_1548_bytes",
          CMAC_OFFSET_STAT_TX_PKT_1523_1548_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_1523_1548_BYTES(1)),
    _STAT("stat_tx_pkt_1549_2047_bytes",
          CMAC_OFFSET_STAT_TX_PKT_1549_2047_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_1549_2047_BYTES(1)),
    _STAT("stat_tx_pkt_2048_4095_bytes",
          CMAC_OFFSET_STAT_TX_PKT_2048_4095_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_2048_4095_BYTES(1)),
    _STAT("stat_tx_pkt_4096_8191_bytes",
          CMAC_OFFSET_STAT_TX_PKT_4096_8191_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_4096_8191_BYTES(1)),
    _STAT("stat_tx_pkt_8192_9215_bytes",
          CMAC_OFFSET_STAT_TX_PKT_8192_9215_BYTES(0),
          CMAC_OFFSET_STAT_TX_PKT_8192_9215_BYTES(1)),
    _STAT("stat_tx_pkt_large",
          CMAC_OFFSET_STAT_TX_PKT_LARGE(0),
          CMAC_OFFSET_STAT_TX_PKT_LARGE(1)),
    _STAT("stat_tx_pkt_small",
          CMAC_OFFSET_STAT_TX_PKT_SMALL(0),
          CMAC_OFFSET_STAT_TX_PKT_SMALL(1)),
    _STAT("stat_tx_bad_fcs",
          CMAC_OFFSET_STAT_TX_BAD_FCS(0),
          CMAC_OFFSET_STAT_TX_BAD_FCS(1)),
    _STAT("stat_tx_unicast",
          CMAC_OFFSET_STAT_TX_UNICAST(0),
          CMAC_OFFSET_STAT_TX_UNICAST(1)),
    _STAT("stat_tx_multicast",
          CMAC_OFFSET_STAT_TX_MULTICAST(0),
          CMAC_OFFSET_STAT_TX_MULTICAST(1)),
    _STAT("stat_tx_broadcast",
          CMAC_OFFSET_STAT_TX_BROADCAST(0),
          CMAC_OFFSET_STAT_TX_BROADCAST(1)),
    _STAT("stat_tx_vlan",
          CMAC_OFFSET_STAT_TX_VLAN(0),
          CMAC_OFFSET_STAT_TX_VLAN(1)),
    _STAT("stat_tx_pause",
          CMAC_OFFSET_STAT_TX_PAUSE(0),
          CMAC_OFFSET_STAT_TX_PAUSE(1)),
    _STAT("stat_tx_user_pause",
          CMAC_OFFSET_STAT_TX_USER_PAUSE(0),
          CMAC_OFFSET_STAT_TX_USER_PAUSE(1)),
    _STAT("stat_rx_total_pkts",
          CMAC_OFFSET_STAT_RX_TOTAL_PKTS(0),
          CMAC_OFFSET_STAT_RX_TOTAL_PKTS(1)),
    _STAT("stat_rx_total_good_pkts",
          CMAC_OFFSET_STAT_RX_TOTAL_GOOD_PKTS(0),
          CMAC_OFFSET_STAT_RX_TOTAL_GOOD_PKTS(1)),
    _STAT("stat_rx_total_bytes",
          CMAC_OFFSET_STAT_RX_TOTAL_BYTES(0),
          CMAC_OFFSET_STAT_RX_TOTAL_BYTES(1)),
    _STAT("stat_rx_total_good_bytes",
          CMAC_OFFSET_STAT_RX_TOTAL_GOOD_BYTES(0),
          CMAC_OFFSET_STAT_RX_TOTAL_GOOD_BYTES(1)),
    _STAT("stat_rx_pkt_64_bytes",
          CMAC_OFFSET_STAT_RX_PKT_64_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_64_BYTES(1)),
    _STAT("stat_rx_pkt_65_127_bytes",
          CMAC_OFFSET_STAT_RX_PKT_65_127_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_65_127_BYTES(1)),
    _STAT("stat_rx_pkt_128_255_bytes",
          CMAC_OFFSET_STAT_RX_PKT_128_255_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_128_255_BYTES(1)),
    _STAT("stat_rx_pkt_256_511_bytes",
          CMAC_OFFSET_STAT_RX_PKT_256_511_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_256_511_BYTES(1)),
    _STAT("stat_rx_pkt_512_1023_bytes",
          CMAC_OFFSET_STAT_RX_PKT_512_1023_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_512_1023_BYTES(1)),
    _STAT("stat_rx_pkt_1024_1518_bytes",
          CMAC_OFFSET_STAT_RX_PKT_1024_1518_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_1024_1518_BYTES(1)),
    _STAT("stat_rx_pkt_1519_1522_bytes",
          CMAC_OFFSET_STAT_RX_PKT_1519_1522_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_1519_1522_BYTES(1)),
    _STAT("stat_rx_pkt_1523_1548_bytes",
          CMAC_OFFSET_STAT_RX_PKT_1523_1548_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_1523_1548_BYTES(1)),
    _STAT("stat_rx_pkt_1549_2047_bytes",
          CMAC_OFFSET_STAT_RX_PKT_1549_2047_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_1549_2047_BYTES(1)),
    _STAT("stat_rx_pkt_2048_4095_bytes",
          CMAC_OFFSET_STAT_RX_PKT_2048_4095_BYTES(0),
          CMAC_OFFSET_STAT_RX_PKT_2048_4095_BYTES(1)),
    _STAT("stat_rx_pkt_4096_8191_bytes",
           CMAC_OFFSET_STAT_RX_PKT_4096_8191_BYTES(0),
           CMAC_OFFSET_STAT_RX_PKT_4096_8191_BYTES(1)),
    _STAT("stat_rx_pkt_8192_9215_bytes",
           CMAC_OFFSET_STAT_RX_PKT_8192_9215_BYTES(0),
           CMAC_OFFSET_STAT_RX_PKT_8192_9215_BYTES(1)),
    _STAT("stat_rx_pkt_large",
           CMAC_OFFSET_STAT_RX_PKT_LARGE(0),
           CMAC_OFFSET_STAT_RX_PKT_LARGE(1)),
    _STAT("stat_rx_pkt_small",
           CMAC_OFFSET_STAT_RX_PKT_SMALL(0),
           CMAC_OFFSET_STAT_RX_PKT_SMALL(1)),
    _STAT("stat_rx_undersize",
           CMAC_OFFSET_STAT_RX_UNDERSIZE(0),
           CMAC_OFFSET_STAT_RX_UNDERSIZE(1)),
    _STAT("stat_rx_fragment",
           CMAC_OFFSET_STAT_RX_FRAGMENT(0),
           CMAC_OFFSET_STAT_RX_FRAGMENT(1)),
    _STAT("stat_rx_oversize",
           CMAC_OFFSET_STAT_RX_OVERSIZE(0),
           CMAC_OFFSET_STAT_RX_OVERSIZE(1)),
    _STAT("stat_rx_toolong",
          CMAC_OFFSET_STAT_RX_TOOLONG(0),
          CMAC_OFFSET_STAT_RX_TOOLONG(1)),
    _STAT("stat_rx_jabber",
          CMAC_OFFSET_STAT_RX_JABBER(0),
          CMAC_OFFSET_STAT_RX_JABBER(1)),
    _STAT("stat_rx_bad_fcs",
           CMAC_OFFSET_STAT_RX_BAD_FCS(0),
           CMAC_OFFSET_STAT_RX_BAD_FCS(1)),
    _STAT("stat_rx_pkt_bad_fcs",
          CMAC_OFFSET_STAT_RX_PKT_BAD_FCS(0),
          CMAC_OFFSET_STAT_RX_PKT_BAD_FCS(1)),
    _STAT("stat_rx_stomped_fcs",
           CMAC_OFFSET_STAT_RX_STOMPED_FCS(0),
           CMAC_OFFSET_STAT_RX_STOMPED_FCS(1)),
    _STAT("stat_rx_unicast",
          CMAC_OFFSET_STAT_RX_UNICAST(0),
          CMAC_OFFSET_STAT_RX_UNICAST(1)),
    _STAT("stat_rx_multicast",
          CMAC_OFFSET_STAT_RX_MULTICAST(0),
          CMAC_OFFSET_STAT_RX_MULTICAST(1)),
    _STAT("stat_rx_broadcast",
          CMAC_OFFSET_STAT_RX_BROADCAST(0),
          CMAC_OFFSET_STAT_RX_BROADCAST(1)),
    _STAT("stat_rx_vlan",
          CMAC_OFFSET_STAT_RX_VLAN(0),
          CMAC_OFFSET_STAT_RX_VLAN(1)),
    _STAT("stat_rx_pause",
          CMAC_OFFSET_STAT_RX_PAUSE(0),
          CMAC_OFFSET_STAT_RX_PAUSE(1)),
    _STAT("stat_rx_user_pause",
          CMAC_OFFSET_STAT_RX_USER_PAUSE(0),
          CMAC_OFFSET_STAT_RX_USER_PAUSE(1)),
    _STAT("stat_rx_inrangeerr",
          CMAC_OFFSET_STAT_RX_INRANGEERR(0),
          CMAC_OFFSET_STAT_RX_INRANGEERR(1)),
    _STAT("stat_rx_truncated",
          CMAC_OFFSET_STAT_RX_TRUNCATED(0),
          CMAC_OFFSET_STAT_RX_TRUNCATED(1)),
};

#define ONIC_QUEUE_STATS_LEN 0
#define ONIC_GLOBAL_STATS_LEN ARRAY_SIZE(onic_gstrings_stats)
#define ONIC_STATS_LEN        (ONIC_GLOBAL_STATS_LEN + ONIC_QUEUE_STATS_LEN)

static void onic_get_drvinfo(struct net_device *netdev,
			     struct ethtool_drvinfo *drvinfo)
{
	printk("onic_get_drvinfo\n");
	struct onic_private *priv = netdev_priv(netdev);

	strlcpy(drvinfo->driver, onic_drv_name, sizeof(drvinfo->driver));
	strlcpy(drvinfo->version, onic_drv_ver,
		sizeof(drvinfo->version));
	strlcpy(drvinfo->bus_info, pci_name(priv->pdev),
		sizeof(drvinfo->bus_info));
}

static u32 onic_get_link(struct net_device *netdev)
{
    u32 val, carrier_ok;
    u8 cmac_idx;
    struct onic_private *priv = netdev_priv(netdev);
    struct onic_hardware *hw = &priv->hw;

    cmac_idx = test_bit(ONIC_FLAG_MASTER_PF, priv->flags) ? 0 : 1;

    /* read twice to flush any previously latched value */
    val = onic_read_reg(hw, CMAC_OFFSET_STAT_RX_STATUS(cmac_idx));
    val = onic_read_reg(hw, CMAC_OFFSET_STAT_RX_STATUS(cmac_idx));

    carrier_ok = netif_carrier_ok(netdev);
    /* verify RX status reg bits are 0x3*/
    val = (val == 0x3);
    
    netdev_info(netdev, "ethtool: onic_get_link port: %d   carrier ok: %u -- "
		    "rx status ok: %u\r\n", cmac_idx, carrier_ok, val);

    return (carrier_ok && val);
}

static void onic_get_ethtool_stats(struct net_device *netdev,
            struct ethtool_stats /*__always_unused*/ *stats,
            u64 *data)
{
    struct onic_private *priv = netdev_priv(netdev);
    struct onic_hardware *hw = &priv->hw;
    struct pci_dev *pdev = priv->pdev;
    struct onic_rx_queue *q = NULL;
    struct onic_ring *desc_ring = NULL; 
    int qid;
    int i;
    u16 func_id;
    u32 off;

    func_id = PCI_FUNC(pdev->devfn);

    //printk("onic_get_ethtool_stats\n");
    // Note :
    //   write 1 into REG_TICK (offset 0x2B0).
    //   this is WriteOnce/SelfClear (WO/SC).
    //   with this, the cmac system updates all STAT_* registers.
    if (func_id == 0)
         onic_write_reg(hw, CMAC_OFFSET_TICK(0), 1);
    else onic_write_reg(hw, CMAC_OFFSET_TICK(1), 1);

    for (i = 0; i < ONIC_GLOBAL_STATS_LEN; i++) {
	    if (onic_gstrings_stats[i].type== ONIC_STATS)  {
		if (func_id == 0)
		  off = onic_gstrings_stats[i].stat0_offset;
		else off = onic_gstrings_stats[i].stat1_offset;
		data[i] = onic_read_reg(hw,off);
	    }
	    if (onic_gstrings_stats[i].type== RING_STATS)  {
            data[i] = 0;
            for (qid = 0; qid < priv->num_rx_queues; ++qid) {
                  q = priv->rx_queue[qid];
                  desc_ring = &q->desc_ring;
                  data[i] += *(u32*)(((u8*)(desc_ring))+onic_gstrings_stats[i].stat0_offset);
                  *(u32*)(((u8*)(desc_ring))+onic_gstrings_stats[i].stat0_offset) =0;
            }
          }
          if (onic_gstrings_stats[i].type== NETDEV_STATS)  {
            data[i] = *(u32*)(((u8*)(priv))+onic_gstrings_stats[i].stat0_offset);
            *(u32*)(((u8*)(priv))+onic_gstrings_stats[i].stat0_offset) =0;
          }
    }
    return;
}

static void onic_get_strings(struct net_device *netdev, u32 stringset,
			      u8 *data)
{
	u8 *p = data;
	int i;

    for (i = 0; i < ONIC_GLOBAL_STATS_LEN; i++) {
        memcpy(p, onic_gstrings_stats[i].stat_string,
            ETH_GSTRING_LEN);
        p += ETH_GSTRING_LEN;
    }
}

static int onic_get_sset_count(struct net_device *netdev, int sset)
{
    return ONIC_STATS_LEN;
}

static u32 onic_get_rxfh_indir_size(struct net_device *dev)
{
	//struct onic_private *priv = netdev_priv(dev);

	//return rounddown_pow_of_two(priv->num_rx_queues);
	printk("onic_get_rxfh_indir_size\n");
	return INDIRECTION_TABLE_SIZE;
}

static u32 onic_get_rxfh_key_size(struct net_device *netdev)
{
	printk("onic_get_rxfh_key_size\n");
	return ONIC_EN_RSS_KEY_SIZE;
}

/*static int onic_check_rxfh_func(struct net_device *dev, u8 hfunc)
{
	struct mlx4_en_priv *priv = netdev_priv(dev);

	printk("onic_check_rxfh_func\n");
	// check if requested function is supported by the device //
	// We only have Toeplitz hash function
	if (hfunc == ETH_RSS_HASH_TOP) {
		//if (!(priv->mdev->dev->caps.flags2 & MLX4_DEV_CAP_FLAG2_RSS_TOP))
		//	return -EINVAL;
		//if (!(dev->features & NETIF_F_RXHASH))
		//	en_warn(priv, "Toeplitz hash function should be used in conjunction with RX hashing for optimal performance\n");
		printk("hfunc: %u\n", hfunc);
		printk("onic_check_rxfh_func: return 0\n");
		return 0;
	} 
	//else if (hfunc == ETH_RSS_HASH_XOR) {
	//	if (!(priv->mdev->dev->caps.flags2 & MLX4_DEV_CAP_FLAG2_RSS_XOR))
	//		return -EINVAL;
	//	if (dev->features & NETIF_F_RXHASH)
	//		en_warn(priv, "Enabling both XOR Hash function and RX Hashing can limit RPS functionality\n");
	//	return 0;
	//}
	printk("onic_check_rxfh_func: return -EINVAL\n");
	return -EINVAL;
}
*/
static int onic_get_rxfh(struct net_device *dev, u32 *ring_index, u8 *key,
		u8 *hfunc)
{
	printk("onic_get_rxfh\n");
	printk("ring_index: %p, key: %p, hfunc: %p\n", ring_index, key, hfunc);
	struct onic_private *priv = netdev_priv(dev);
	u32 n = onic_get_rxfh_indir_size(dev);
	u32 i;
	u32 base_address = INDIRECTION_TABLE_BASE_ADDR;

	if (ring_index) {
		printk("ring_index\n");
		for (i = 0; i < n; i++) {
			ring_index[i] = 0xFFFF & onic_read_reg(&priv->hw, base_address + i * 4);
			printk("offset: %x, val: %x\n", base_address + i * 4, ring_index[i]);
		}
	}

	if (key) {
		base_address = 0x1600;
		printk("key\n");
		for (i = 0; i < ONIC_EN_RSS_KEY_SIZE/4; i++) {
			u32 val = onic_read_reg(&priv->hw, base_address + i*4);
			memcpy(&key[i*4],&val, 4);
			printk("offset: %x, val: %x\n", base_address + i*4, val);
		}
	}

	if (hfunc)
		*hfunc = ETH_RSS_HASH_TOP;

	printk("onic_get_rxfh: return 0\n");

	return 0;
}

static int onic_set_rxfh(struct net_device *dev, const u32 *ring_index,
			    const u8 *key, const u8 hfunc)
{

	printk("onic_set_rxfh\n");
	printk("ring_index: %p, key: %p, hfunc: %u\n", ring_index, key, hfunc);
	struct onic_private *priv = netdev_priv(dev);
	int n = onic_get_rxfh_indir_size(dev);
	int i=0;
	///* Calculate RSS table size and make sure flows are spread evenly
	// * between rings
	// */
	// Check that we have a valid ring index
	if (!ring_index) {
		printk("onic_set_rxfh: return -EINVAL\n");
		printk("!ring_index\n");
		return -EINVAL;
	}
	printk("ring_index: %p\n", ring_index);
	for (i = 0; i < n; i++) {
		// Check that the ring index is within the number of rx queues
		if (ring_index[i] >= priv->num_rx_queues) {
			printk("onic_set_rxfh: return -EINVAL\n");
			printk("ring_index >= priv->num_rx_queues\n");
			return -EINVAL;
		}
		onic_write_reg(&priv->hw, INDIRECTION_TABLE_BASE_ADDR + i * 4, ring_index[i]);
		printk("offset: %x, val: %x\n", INDIRECTION_TABLE_BASE_ADDR + i * 4, ring_index[i]);
	}

	printk("key: %p\n", key);
	if (!key) {
		//printk("onic_set_rxfh: return -EINVAL\n");
		printk("!key\n");
		//return -EINVAL;
	}
	else for (i = 0; i < ONIC_EN_RSS_KEY_SIZE/4; i++) {
		u32 val;
		memcpy(&val, &key[i*4], 4);
		onic_write_reg(&priv->hw, 0x1600 + i*4, val);
		printk("offset: %x, val: %x\n", 0x1600 + i*4, val);
	}
	// Cannot change hash function
	return 0;
}

int onic_get_rxnfc(struct net_device *dev, struct ethtool_rxnfc *info, u32 *rule_locs) {
	struct onic_private *priv = netdev_priv(dev);
	printk("onic_get_rxnfc\n");
	info->data = priv->num_rx_queues;
	return 0;
}


static const struct ethtool_ops onic_ethtool_ops = {
    .get_drvinfo       = onic_get_drvinfo,
    .get_link          = onic_get_link,
    .get_ethtool_stats = onic_get_ethtool_stats,
    .get_strings       = onic_get_strings,
    .get_sset_count    = onic_get_sset_count,
    .get_rxfh_indir_size = onic_get_rxfh_indir_size,
    .get_rxfh_key_size = onic_get_rxfh_key_size,
    .get_rxfh = onic_get_rxfh,
    .set_rxfh = onic_set_rxfh,
    .get_rxnfc = onic_get_rxnfc,

};

void onic_set_ethtool_ops(struct net_device *netdev)
{
    netdev->ethtool_ops = &onic_ethtool_ops;
}
