/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 1999 - 2018 Intel Corporation. */

#ifndef _E1000E_MANAGE_H_
#define _E1000E_MANAGE_H_

bool e1000e_check_mng_mode_generic(struct e1000_hw *hw);
bool e1000e_enable_tx_pkt_filtering(struct e1000_hw *hw);
s32 e1000e_mng_write_dhcp_info(struct e1000_hw *hw, u8 *buffer, u16 length);
bool e1000e_enable_mng_pass_thru(struct e1000_hw *hw);

enum e1000_mng_mode {
e1000_mng_mode_none = 0,
e1000_mng_mode_asf,
e1000_mng_mode_pt,
e1000_mng_mode_ipmi,
e1000_mng_mode_host_if_only
};

#define E1000_FACTPS_MNGCG0x20000000

#define E1000_FWSM_MODE_MASK0xE
#define E1000_FWSM_MODE_SHIFT1

#define E1000_MNG_IAMT_MODE0x3
#define E1000_MNG_DHCP_COOKIE_LENGTH0x10
#define E1000_MNG_DHCP_COOKIE_OFFSET0x6F0
#define E1000_MNG_DHCP_COMMAND_TIMEOUT10
#define E1000_MNG_DHCP_TX_PAYLOAD_CMD64
#define E1000_MNG_DHCP_COOKIE_STATUS_PARSING0x1
#define E1000_MNG_DHCP_COOKIE_STATUS_VLAN0x2

#define E1000_VFTA_ENTRY_SHIFT5
#define E1000_VFTA_ENTRY_MASK0x7F
#define E1000_VFTA_ENTRY_BIT_SHIFT_MASK0x1F

#define E1000_HICR_EN0x01/* Enable bit - RO */
/* Driver sets this bit when done to put command in RAM */
#define E1000_HICR_C0x02
#define E1000_HICR_SV0x04/* Status Validity */
#define E1000_HICR_FW_RESET_ENABLE0x40
#define E1000_HICR_FW_RESET0x80

/* Intel(R) Active Management Technology signature */
#define E1000_IAMT_SIGNATURE0x544D4149

#endif
