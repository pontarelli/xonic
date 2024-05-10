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
#ifndef __ONIC_REGISTER_H__
#define __ONIC_REGISTER_H__

#include "onic_hardware.h"

static inline u32 onic_read_reg(struct onic_hardware *hw, u32 offset)
{
	return ioread32(hw->addr + offset);
}

static inline void onic_write_reg(struct onic_hardware *hw, u32 offset, u32 val)
{
	iowrite32(val, hw->addr + offset);
}

#define SHELL_START					0x0
#define SHELL_END					0x400000 /* include CMS register space, 0x320000 to 0x330000 */
#define SHELL_MAXLEN					(SHELL_END - SHELL_START)

#define P250_OFFSET                                     0x100000
#define P250_CONTROL_REG                                (P250_OFFSET + 0x0)


/***** system config registers *****/
#define SYSCFG_OFFSET					0x0

#define SYSCFG_OFFSET_BUILD_STATUS			(SYSCFG_OFFSET + 0x0)
#define SYSCFG_OFFSET_SYSTEM_RESET			(SYSCFG_OFFSET + 0x4)
#define SYSCFG_OFFSET_SYSTEM_STATUS			(SYSCFG_OFFSET + 0x8)
#define SYSCFG_OFFSET_SHELL_RESET			(SYSCFG_OFFSET + 0xC)
#define SYSCFG_OFFSET_SHELL_STATUS			(SYSCFG_OFFSET + 0x10)
#define SYSCFG_OFFSET_USER_RESET			(SYSCFG_OFFSET + 0x14)
#define SYSCFG_OFFSET_USER_STATUS			(SYSCFG_OFFSET + 0x18)

/***** QDMA subsystem registers *****/
#define QDMA_SUBSYSTEM_OFFSET				0x1000
#define QDMA_FUNC_OFFSET(i)				(QDMA_SUBSYSTEM_OFFSET + (0x1000 * i))
#define QDMA_SUBSYS_OFFSET				(QDMA_SUBSYSTEM_OFFSET + 0x4000)

#define QDMA_FUNC_OFFSET_QCONF(i)			((QDMA_FUNC_OFFSET(i)) + 0x0)
#define     QDMA_FUNC_QCONF_QBASE_MASK			GENMASK(31, 16)
#define     QDMA_FUNC_QCONF_NUMQ_MASK			GENMASK(15, 0)
#define QDMA_FUNC_OFFSET_INDIR_TABLE(i, k)		((QDMA_FUNC_OFFSET(i)) + 0x400 + ((k) * 4))
#define QDMA_FUNC_OFFSET_HASH_KEY(i, k)			((QDMA_FUNC_OFFSET(i)) + 0x600 + ((k) * 4))
#define QDMA_FUNC_CONTROL_REG(i)                        ((QDMA_FUNC_OFFSET(i)) + 0x910)

/***** CMAC subsystem registers *****/
#define CMAC_SUBSYSTEM_0_OFFSET				0x8000
#define CMAC_SUBSYSTEM_1_OFFSET				0xC000
#define CMAC_SUBSYSTEM_OFFSET(i)			(((i) == 0) ? CMAC_SUBSYSTEM_0_OFFSET : CMAC_SUBSYSTEM_1_OFFSET)

#define CMAC_OFFSET(i)					(CMAC_SUBSYSTEM_OFFSET(i) + 0x0)
#define CMAC_QSFP_OFFSET(i)				(CMAC_SUBSYSTEM_OFFSET(i) + 0x2000)
#define CMAC_ADPT_OFFSET(i)				(CMAC_SUBSYSTEM_OFFSET(i) + 0x3000)

/* CMAC Configuration Registers */
#define CMAC_OFFSET_GT_RESET(i)				(CMAC_OFFSET(i) + 0x0000)
#define CMAC_OFFSET_RESET(i)				(CMAC_OFFSET(i) + 0x0004)
#define CMAC_OFFSET_SWITCH_CORE_MODE(i)			(CMAC_OFFSET(i) + 0x0008)

#define CMAC_OFFSET_CONF_TX_1(i)			(CMAC_OFFSET(i) + 0x000C)
#define CMAC_OFFSET_CONF_RX_1(i)			(CMAC_OFFSET(i) + 0x0014)
#define CMAC_OFFSET_CORE_MODE(i)			(CMAC_OFFSET(i) + 0x0020)
#define CMAC_OFFSET_CORE_VERSION(i)			(CMAC_OFFSET(i) + 0x0024)

#define CMAC_OFFSET_CONF_TX_BIP_OVERRIDE(i)		(CMAC_OFFSET(i) + 0x002C)
#define CMAC_OFFSET_CONF_TX_FC_CTRL_1(i)		(CMAC_OFFSET(i) + 0x0030)
#define CMAC_OFFSET_CONF_TX_FC_RFRH_1(i)		(CMAC_OFFSET(i) + 0x0034)
#define CMAC_OFFSET_CONF_TX_FC_RFRH_2(i)		(CMAC_OFFSET(i) + 0x0038)
#define CMAC_OFFSET_CONF_TX_FC_RFRH_3(i)		(CMAC_OFFSET(i) + 0x003C)
#define CMAC_OFFSET_CONF_TX_FC_RFRH_4(i)		(CMAC_OFFSET(i) + 0x0040)
#define CMAC_OFFSET_CONF_TX_FC_RFRH_5(i)		(CMAC_OFFSET(i) + 0x0044)
#define CMAC_OFFSET_CONF_TX_FC_QNTA_1(i)		(CMAC_OFFSET(i) + 0x0048)
#define CMAC_OFFSET_CONF_TX_FC_QNTA_2(i)		(CMAC_OFFSET(i) + 0x004C)
#define CMAC_OFFSET_CONF_TX_FC_QNTA_3(i)		(CMAC_OFFSET(i) + 0x0050)
#define CMAC_OFFSET_CONF_TX_FC_QNTA_4(i)		(CMAC_OFFSET(i) + 0x0054)
#define CMAC_OFFSET_CONF_TX_FC_QNTA_5(i)		(CMAC_OFFSET(i) + 0x0058)
#define CMAC_OFFSET_CONF_TX_OTN_PKT_LEN(i)		(CMAC_OFFSET(i) + 0x005C)
#define CMAC_OFFSET_CONF_TX_OTN_CTRL(i)			(CMAC_OFFSET(i) + 0x0060)

#define CMAC_OFFSET_CONF_RX_FC_CTRL_1(i)		(CMAC_OFFSET(i) + 0x0084)
#define CMAC_OFFSET_CONF_RX_FC_CTRL_2(i)		(CMAC_OFFSET(i) + 0x0088)
#define CMAC_OFFSET_GT_LOOPBACK(i)			(CMAC_OFFSET(i) + 0x0090)

#define CMAC_OFFSET_CONF_AN_CTRL_1(i)			(CMAC_OFFSET(i) + 0x00A0)
#define CMAC_OFFSET_CONF_AN_CTRL_2(i)			(CMAC_OFFSET(i) + 0x00A4)
#define CMAC_OFFSET_CONF_AN_ABILITY(i)			(CMAC_OFFSET(i) + 0x00A8)
#define CMAC_OFFSET_CONF_LT_CTRL_1(i)			(CMAC_OFFSET(i) + 0x00AC)
#define CMAC_OFFSET_CONF_LT_TRAINED(i)			(CMAC_OFFSET(i) + 0x00B0)
#define CMAC_OFFSET_CONF_LT_PRESET(i)			(CMAC_OFFSET(i) + 0x00B4)
#define CMAC_OFFSET_CONF_LT_INIT(i)			(CMAC_OFFSET(i) + 0x00B8)
#define CMAC_OFFSET_CONF_LT_SEED_0(i)			(CMAC_OFFSET(i) + 0x00BC)
#define CMAC_OFFSET_CONF_LT_SEED_1(i)			(CMAC_OFFSET(i) + 0x00C0)
#define CMAC_OFFSET_CONF_LT_COEF_0(i)			(CMAC_OFFSET(i) + 0x00C4)
#define CMAC_OFFSET_CONF_LT_COEF_1(i)			(CMAC_OFFSET(i) + 0x00C8)
#define CMAC_OFFSET_USER_0(i)				(CMAC_OFFSET(i) + 0x00CC)

#define CMAC_OFFSET_RSFEC_CONF_IND_CORRECTION(i)	(CMAC_OFFSET(i) + 0x1000)
#define CMAC_OFFSET_RSFEC_CONF_ENABLE(i)		(CMAC_OFFSET(i) + 0x107C)

/* CMAC Status Registers */
#define CMAC_OFFSET_STAT_TX_STATUS(i)			(CMAC_OFFSET(i) + 0x0200)
#define CMAC_OFFSET_STAT_RX_STATUS(i)			(CMAC_OFFSET(i) + 0x0204)
#define CMAC_OFFSET_STAT_STATUS_1(i)			(CMAC_OFFSET(i) + 0x0208)
#define CMAC_OFFSET_STAT_RX_BLOCK_LOCK(i)		(CMAC_OFFSET(i) + 0x020C)
#define CMAC_OFFSET_STAT_RX_LANE_SYNC(i)		(CMAC_OFFSET(i) + 0x0210)
#define CMAC_OFFSET_STAT_RX_LANE_SYNC_ERR(i)		(CMAC_OFFSET(i) + 0x0214)
#define CMAC_OFFSET_STAT_RX_AM_ERR(i)			(CMAC_OFFSET(i) + 0x0218)
#define CMAC_OFFSET_STAT_RX_AM_LEN_ERR(i)		(CMAC_OFFSET(i) + 0x021C)
#define CMAC_OFFSET_STAT_RX_AM_REPEAT_ERR(i)		(CMAC_OFFSET(i) + 0x0220)
#define CMAC_OFFSET_STAT_RX_PCSL_DEMUXED(i)		(CMAC_OFFSET(i) + 0x0224)
#define CMAC_OFFSET_STAT_RX_PCS_LANE_NUM_1(i)		(CMAC_OFFSET(i) + 0x0228)
#define CMAC_OFFSET_STAT_RX_PCS_LANE_NUM_2(i)		(CMAC_OFFSET(i) + 0x022C)
#define CMAC_OFFSET_STAT_RX_PCS_LANE_NUM_3(i)		(CMAC_OFFSET(i) + 0x0230)
#define CMAC_OFFSET_STAT_RX_PCS_LANE_NUM_4(i)		(CMAC_OFFSET(i) + 0x0234)
#define CMAC_OFFSET_STAT_RX_BIP_OVERRIDE(i)		(CMAC_OFFSET(i) + 0x0238)
#define CMAC_OFFSET_STAT_TX_OTN_STATUS(i)		(CMAC_OFFSET(i) + 0x023C)
#define CMAC_OFFSET_STAT_AN_STATUS(i)			(CMAC_OFFSET(i) + 0x0258)
#define CMAC_OFFSET_STAT_AN_ABILITY(i)			(CMAC_OFFSET(i) + 0x025C)
#define CMAC_OFFSET_STAT_AN_LINK_CTL_1(i)		(CMAC_OFFSET(i) + 0x0260)
#define CMAC_OFFSET_STAT_LT_STATUS_1(i)			(CMAC_OFFSET(i) + 0x0264)
#define CMAC_OFFSET_STAT_LT_STATUS_2(i)			(CMAC_OFFSET(i) + 0x0268)
#define CMAC_OFFSET_STAT_LT_STATUS_3(i)			(CMAC_OFFSET(i) + 0x026C)
#define CMAC_OFFSET_STAT_LT_STATUS_4(i)			(CMAC_OFFSET(i) + 0x0270)
#define CMAC_OFFSET_STAT_LT_COEF_0(i)			(CMAC_OFFSET(i) + 0x0274)
#define CMAC_OFFSET_STAT_LT_COEF_1(i)			(CMAC_OFFSET(i) + 0x0278)
#define CMAC_OFFSET_STAT_AN_LINK_CTL_2(i)		(CMAC_OFFSET(i) + 0x027C)
#define CMAC_OFFSET_STAT_RSFEC_STATUS(i)		(CMAC_OFFSET(i) + 0x1004)
#define CMAC_OFFSET_STAT_RSFEC_LANE_MAPPING(i)		(CMAC_OFFSET(i) + 0x1018)
#define CMAC_OFFSET_STAT_TX_OTN_RSFEC_STATUS(i)		(CMAC_OFFSET(i) + 0x1044)

#define CMAC_OFFSET_TICK(i)				(CMAC_OFFSET(i) + 0x02B0)
#define CMAC_OFFSET_STAT_CYCLE_COUNT(i)			(CMAC_OFFSET(i) + 0x02B8)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_0(i)		(CMAC_OFFSET(i) + 0x02C0)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_1(i)		(CMAC_OFFSET(i) + 0x02C8)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_2(i)		(CMAC_OFFSET(i) + 0x02D0)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_3(i)		(CMAC_OFFSET(i) + 0x02D8)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_4(i)		(CMAC_OFFSET(i) + 0x02E0)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_5(i)		(CMAC_OFFSET(i) + 0x02E8)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_6(i)		(CMAC_OFFSET(i) + 0x02F0)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_7(i)		(CMAC_OFFSET(i) + 0x02F8)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_8(i)		(CMAC_OFFSET(i) + 0x0300)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_9(i)		(CMAC_OFFSET(i) + 0x0308)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_10(i)		(CMAC_OFFSET(i) + 0x0310)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_11(i)		(CMAC_OFFSET(i) + 0x0318)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_12(i)		(CMAC_OFFSET(i) + 0x0320)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_13(i)		(CMAC_OFFSET(i) + 0x0328)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_14(i)		(CMAC_OFFSET(i) + 0x0330)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_15(i)		(CMAC_OFFSET(i) + 0x0338)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_16(i)		(CMAC_OFFSET(i) + 0x0340)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_17(i)		(CMAC_OFFSET(i) + 0x0348)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_18(i)		(CMAC_OFFSET(i) + 0x0350)
#define CMAC_OFFSET_STAT_RX_BIP_ERR_19(i)		(CMAC_OFFSET(i) + 0x0358)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_0(i)		(CMAC_OFFSET(i) + 0x0360)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_1(i)		(CMAC_OFFSET(i) + 0x0368)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_2(i)		(CMAC_OFFSET(i) + 0x0370)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_3(i)		(CMAC_OFFSET(i) + 0x0378)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_4(i)		(CMAC_OFFSET(i) + 0x0380)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_5(i)		(CMAC_OFFSET(i) + 0x0388)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_6(i)		(CMAC_OFFSET(i) + 0x0390)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_7(i)		(CMAC_OFFSET(i) + 0x0398)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_8(i)		(CMAC_OFFSET(i) + 0x03A0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_9(i)		(CMAC_OFFSET(i) + 0x03A8)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_10(i)		(CMAC_OFFSET(i) + 0x03B0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_11(i)		(CMAC_OFFSET(i) + 0x03B8)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_12(i)		(CMAC_OFFSET(i) + 0x03C0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_13(i)		(CMAC_OFFSET(i) + 0x03C8)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_14(i)		(CMAC_OFFSET(i) + 0x03D0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_15(i)		(CMAC_OFFSET(i) + 0x03D8)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_16(i)		(CMAC_OFFSET(i) + 0x03E0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_17(i)		(CMAC_OFFSET(i) + 0x03E8)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_18(i)		(CMAC_OFFSET(i) + 0x03F0)
#define CMAC_OFFSET_STAT_RX_FRAMING_ERR_19(i)		(CMAC_OFFSET(i) + 0x03F8)
#define CMAC_OFFSET_STAT_RX_BAD_CODE(i)			(CMAC_OFFSET(i) + 0x0418)
#define CMAC_OFFSET_STAT_TX_FRAME_ERROR(i)		(CMAC_OFFSET(i) + 0x0458)
#define CMAC_OFFSET_STAT_TX_TOTAL_PKTS(i)		(CMAC_OFFSET(i) + 0x0500)
#define CMAC_OFFSET_STAT_TX_TOTAL_GOOD_PKTS(i)		(CMAC_OFFSET(i) + 0x0508)
#define CMAC_OFFSET_STAT_TX_TOTAL_BYTES(i)		(CMAC_OFFSET(i) + 0x0510)
#define CMAC_OFFSET_STAT_TX_TOTAL_GOOD_BYTES(i)		(CMAC_OFFSET(i) + 0x0518)
#define CMAC_OFFSET_STAT_TX_PKT_64_BYTES(i)		(CMAC_OFFSET(i) + 0x0520)
#define CMAC_OFFSET_STAT_TX_PKT_65_127_BYTES(i)		(CMAC_OFFSET(i) + 0x0528)
#define CMAC_OFFSET_STAT_TX_PKT_128_255_BYTES(i)	(CMAC_OFFSET(i) + 0x0530)
#define CMAC_OFFSET_STAT_TX_PKT_256_511_BYTES(i)	(CMAC_OFFSET(i) + 0x0538)
#define CMAC_OFFSET_STAT_TX_PKT_512_1023_BYTES(i)	(CMAC_OFFSET(i) + 0x0540)
#define CMAC_OFFSET_STAT_TX_PKT_1024_1518_BYTES(i)	(CMAC_OFFSET(i) + 0x0548)
#define CMAC_OFFSET_STAT_TX_PKT_1519_1522_BYTES(i)	(CMAC_OFFSET(i) + 0x0550)
#define CMAC_OFFSET_STAT_TX_PKT_1523_1548_BYTES(i)	(CMAC_OFFSET(i) + 0x0558)
#define CMAC_OFFSET_STAT_TX_PKT_1549_2047_BYTES(i)	(CMAC_OFFSET(i) + 0x0560)
#define CMAC_OFFSET_STAT_TX_PKT_2048_4095_BYTES(i)	(CMAC_OFFSET(i) + 0x0568)
#define CMAC_OFFSET_STAT_TX_PKT_4096_8191_BYTES(i)	(CMAC_OFFSET(i) + 0x0570)
#define CMAC_OFFSET_STAT_TX_PKT_8192_9215_BYTES(i)	(CMAC_OFFSET(i) + 0x0578)
#define CMAC_OFFSET_STAT_TX_PKT_LARGE(i)		(CMAC_OFFSET(i) + 0x0580)
#define CMAC_OFFSET_STAT_TX_PKT_SMALL(i)		(CMAC_OFFSET(i) + 0x0588)
#define CMAC_OFFSET_STAT_TX_BAD_FCS(i)			(CMAC_OFFSET(i) + 0x05B8)
#define CMAC_OFFSET_STAT_TX_UNICAST(i)			(CMAC_OFFSET(i) + 0x05D0)
#define CMAC_OFFSET_STAT_TX_MULTICAST(i)		(CMAC_OFFSET(i) + 0x05D8)
#define CMAC_OFFSET_STAT_TX_BROADCAST(i)		(CMAC_OFFSET(i) + 0x05E0)
#define CMAC_OFFSET_STAT_TX_VLAN(i)			(CMAC_OFFSET(i) + 0x05E8)
#define CMAC_OFFSET_STAT_TX_PAUSE(i)			(CMAC_OFFSET(i) + 0x05F0)
#define CMAC_OFFSET_STAT_TX_USER_PAUSE(i)		(CMAC_OFFSET(i) + 0x05F8)
#define CMAC_OFFSET_STAT_RX_TOTAL_PKTS(i)		(CMAC_OFFSET(i) + 0x0608)
#define CMAC_OFFSET_STAT_RX_TOTAL_GOOD_PKTS(i)		(CMAC_OFFSET(i) + 0x0610)
#define CMAC_OFFSET_STAT_RX_TOTAL_BYTES(i)		(CMAC_OFFSET(i) + 0x0618)
#define CMAC_OFFSET_STAT_RX_TOTAL_GOOD_BYTES(i)		(CMAC_OFFSET(i) + 0x0620)
#define CMAC_OFFSET_STAT_RX_PKT_64_BYTES(i)		(CMAC_OFFSET(i) + 0x0628)
#define CMAC_OFFSET_STAT_RX_PKT_65_127_BYTES(i)		(CMAC_OFFSET(i) + 0x0630)
#define CMAC_OFFSET_STAT_RX_PKT_128_255_BYTES(i)	(CMAC_OFFSET(i) + 0x0638)
#define CMAC_OFFSET_STAT_RX_PKT_256_511_BYTES(i)	(CMAC_OFFSET(i) + 0x0640)
#define CMAC_OFFSET_STAT_RX_PKT_512_1023_BYTES(i)	(CMAC_OFFSET(i) + 0x0648)
#define CMAC_OFFSET_STAT_RX_PKT_1024_1518_BYTES(i)	(CMAC_OFFSET(i) + 0x0650)
#define CMAC_OFFSET_STAT_RX_PKT_1519_1522_BYTES(i)	(CMAC_OFFSET(i) + 0x0658)
#define CMAC_OFFSET_STAT_RX_PKT_1523_1548_BYTES(i)	(CMAC_OFFSET(i) + 0x0660)
#define CMAC_OFFSET_STAT_RX_PKT_1549_2047_BYTES(i)	(CMAC_OFFSET(i) + 0x0668)
#define CMAC_OFFSET_STAT_RX_PKT_2048_4095_BYTES(i)	(CMAC_OFFSET(i) + 0x0670)
#define CMAC_OFFSET_STAT_RX_PKT_4096_8191_BYTES(i)	(CMAC_OFFSET(i) + 0x0678)
#define CMAC_OFFSET_STAT_RX_PKT_8192_9215_BYTES(i)	(CMAC_OFFSET(i) + 0x0680)
#define CMAC_OFFSET_STAT_RX_PKT_LARGE(i)		(CMAC_OFFSET(i) + 0x0688)
#define CMAC_OFFSET_STAT_RX_PKT_SMALL(i)		(CMAC_OFFSET(i) + 0x0690)
#define CMAC_OFFSET_STAT_RX_UNDERSIZE(i)		(CMAC_OFFSET(i) + 0x0698)
#define CMAC_OFFSET_STAT_RX_FRAGMENT(i)			(CMAC_OFFSET(i) + 0x06A0)
#define CMAC_OFFSET_STAT_RX_OVERSIZE(i)			(CMAC_OFFSET(i) + 0x06A8)
#define CMAC_OFFSET_STAT_RX_TOOLONG(i)			(CMAC_OFFSET(i) + 0x06B0)
#define CMAC_OFFSET_STAT_RX_JABBER(i)			(CMAC_OFFSET(i) + 0x06B8)
#define CMAC_OFFSET_STAT_RX_BAD_FCS(i)			(CMAC_OFFSET(i) + 0x06C0)
#define CMAC_OFFSET_STAT_RX_PKT_BAD_FCS(i)		(CMAC_OFFSET(i) + 0x06C8)
#define CMAC_OFFSET_STAT_RX_STOMPED_FCS(i)		(CMAC_OFFSET(i) + 0x06D0)
#define CMAC_OFFSET_STAT_RX_UNICAST(i)			(CMAC_OFFSET(i) + 0x06D8)
#define CMAC_OFFSET_STAT_RX_MULTICAST(i)		(CMAC_OFFSET(i) + 0x06E0)
#define CMAC_OFFSET_STAT_RX_BROADCAST(i)		(CMAC_OFFSET(i) + 0x06E8)
#define CMAC_OFFSET_STAT_RX_VLAN(i)			(CMAC_OFFSET(i) + 0x06F0)
#define CMAC_OFFSET_STAT_RX_PAUSE(i)			(CMAC_OFFSET(i) + 0x06F8)
#define CMAC_OFFSET_STAT_RX_USER_PAUSE(i)		(CMAC_OFFSET(i) + 0x0700)
#define CMAC_OFFSET_STAT_RX_INRANGEERR(i)		(CMAC_OFFSET(i) + 0x0708)
#define CMAC_OFFSET_STAT_RX_TRUNCATED(i)		(CMAC_OFFSET(i) + 0x0710)
#define CMAC_OFFSET_STAT_OTN_TX_JABBER(i)		(CMAC_OFFSET(i) + 0x0718)
#define CMAC_OFFSET_STAT_OTN_TX_OVERSIZE(i)		(CMAC_OFFSET(i) + 0x0720)
#define CMAC_OFFSET_STAT_OTN_TX_UNDERSIZE(i)		(CMAC_OFFSET(i) + 0x0728)
#define CMAC_OFFSET_STAT_OTN_TX_TOOLONG(i)		(CMAC_OFFSET(i) + 0x0730)
#define CMAC_OFFSET_STAT_OTN_TX_FRAGMENT(i)		(CMAC_OFFSET(i) + 0x0738)
#define CMAC_OFFSET_STAT_OTN_TX_PKT_BAD_FCS(i)		(CMAC_OFFSET(i) + 0x0740)
#define CMAC_OFFSET_STAT_OTN_TX_STOMPED_FCS(i)		(CMAC_OFFSET(i) + 0x0748)
#define CMAC_OFFSET_STAT_OTN_TX_BAD_CODE(i)		(CMAC_OFFSET(i) + 0x0750)
#define CMAC_OFFSET_STAT_RX_RSFEC_CORRECTED_CW_INC(i)	(CMAC_OFFSET(i) + 0x1008)
#define CMAC_OFFSET_STAT_RX_RSFEC_UNCORRECTED_CW_INC(i)	(CMAC_OFFSET(i) + 0x1010)
#define CMAC_OFFSET_STAT_RX_RSFEC_ERR_COUNT0_INC(i)	(CMAC_OFFSET(i) + 0x101C)
#define CMAC_OFFSET_STAT_RX_RSFEC_ERR_COUNT1_INC(i)	(CMAC_OFFSET(i) + 0x1024)
#define CMAC_OFFSET_STAT_RX_RSFEC_ERR_COUNT2_INC(i)	(CMAC_OFFSET(i) + 0x102C)
#define CMAC_OFFSET_STAT_RX_RSFEC_ERR_COUNT3_INC(i)	(CMAC_OFFSET(i) + 0x1034)
#define CMAC_OFFSET_STAT_RX_RSFEC_CW_INC(i)		(CMAC_OFFSET(i) + 0x103C)

#define CMAC_ADPT_OFFSET_TX_PKT_RECV(i)			(CMAC_ADPT_OFFSET(i) + 0x0)
#define CMAC_ADPT_OFFSET_TX_PKT_DROP(i)			(CMAC_ADPT_OFFSET(i) + 0x4)
#define CMAC_ADPT_OFFSET_RX_PKT_RECV(i)			(CMAC_ADPT_OFFSET(i) + 0x10)
#define CMAC_ADPT_OFFSET_RX_PKT_DROP(i)			(CMAC_ADPT_OFFSET(i) + 0x14)
#define CMAC_ADPT_OFFSET_RX_PKT_ERROR(i)		(CMAC_ADPT_OFFSET(i) + 0x18)

/* INDIRECTION TABLE */
#define INDIRECTION_TABLE_BASE_ADDR			0x1400
#define INDIRECTION_TABLE_SIZE				0x80
#endif
