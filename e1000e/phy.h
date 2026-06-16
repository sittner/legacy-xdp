/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 1999 - 2018 Intel Corporation. */

#ifndef _E1000E_PHY_H_
#define _E1000E_PHY_H_

s32 e1000e_check_downshift(struct e1000_hw *hw);
s32 e1000_check_polarity_m88(struct e1000_hw *hw);
s32 e1000_check_polarity_igp(struct e1000_hw *hw);
s32 e1000_check_polarity_ife(struct e1000_hw *hw);
s32 e1000e_check_reset_block_generic(struct e1000_hw *hw);
s32 e1000e_copper_link_setup_igp(struct e1000_hw *hw);
s32 e1000e_copper_link_setup_m88(struct e1000_hw *hw);
s32 e1000e_phy_force_speed_duplex_igp(struct e1000_hw *hw);
s32 e1000e_phy_force_speed_duplex_m88(struct e1000_hw *hw);
s32 e1000_phy_force_speed_duplex_ife(struct e1000_hw *hw);
s32 e1000e_get_cable_length_m88(struct e1000_hw *hw);
s32 e1000e_get_cable_length_igp_2(struct e1000_hw *hw);
s32 e1000e_get_cfg_done_generic(struct e1000_hw *hw);
s32 e1000e_get_phy_id(struct e1000_hw *hw);
s32 e1000e_get_phy_info_igp(struct e1000_hw *hw);
s32 e1000e_get_phy_info_m88(struct e1000_hw *hw);
s32 e1000_get_phy_info_ife(struct e1000_hw *hw);
s32 e1000e_phy_sw_reset(struct e1000_hw *hw);
void e1000e_phy_force_speed_duplex_setup(struct e1000_hw *hw, u16 *phy_ctrl);
s32 e1000e_phy_hw_reset_generic(struct e1000_hw *hw);
s32 e1000e_phy_reset_dsp(struct e1000_hw *hw);
s32 e1000e_read_kmrn_reg(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_read_kmrn_reg_locked(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000_set_page_igp(struct e1000_hw *hw, u16 page);
s32 e1000e_read_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_read_phy_reg_igp_locked(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_read_phy_reg_m88(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_set_d3_lplu_state(struct e1000_hw *hw, bool active);
s32 e1000e_setup_copper_link(struct e1000_hw *hw);
s32 e1000e_write_kmrn_reg(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_write_kmrn_reg_locked(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_write_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_write_phy_reg_igp_locked(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_write_phy_reg_m88(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_phy_has_link_generic(struct e1000_hw *hw, u32 iterations,
u32 usec_interval, bool *success);
s32 e1000e_phy_init_script_igp3(struct e1000_hw *hw);
enum e1000_phy_type e1000e_get_phy_type_from_id(u32 phy_id);
s32 e1000e_determine_phy_address(struct e1000_hw *hw);
s32 e1000e_write_phy_reg_bm(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000e_read_phy_reg_bm(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000_enable_phy_wakeup_reg_access_bm(struct e1000_hw *hw, u16 *phy_reg);
s32 e1000_disable_phy_wakeup_reg_access_bm(struct e1000_hw *hw, u16 *phy_reg);
s32 e1000e_read_phy_reg_bm2(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_write_phy_reg_bm2(struct e1000_hw *hw, u32 offset, u16 data);
void e1000_power_up_phy_copper(struct e1000_hw *hw);
void e1000_power_down_phy_copper(struct e1000_hw *hw);
void e1000e_disable_phy_retry(struct e1000_hw *hw);
void e1000e_enable_phy_retry(struct e1000_hw *hw);
s32 e1000e_read_phy_reg_mdic(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000e_write_phy_reg_mdic(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000_read_phy_reg_hv(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000_read_phy_reg_hv_locked(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000_read_phy_reg_page_hv(struct e1000_hw *hw, u32 offset, u16 *data);
s32 e1000_write_phy_reg_hv(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000_write_phy_reg_hv_locked(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000_write_phy_reg_page_hv(struct e1000_hw *hw, u32 offset, u16 data);
s32 e1000_link_stall_workaround_hv(struct e1000_hw *hw);
s32 e1000_copper_link_setup_82577(struct e1000_hw *hw);
s32 e1000_check_polarity_82577(struct e1000_hw *hw);
s32 e1000_get_phy_info_82577(struct e1000_hw *hw);
s32 e1000_phy_force_speed_duplex_82577(struct e1000_hw *hw);
s32 e1000_get_cable_length_82577(struct e1000_hw *hw);

#define E1000_MAX_PHY_ADDR8

/* IGP01E1000 Specific Registers */
#define IGP01E1000_PHY_PORT_CONFIG0x10/* Port Config */
#define IGP01E1000_PHY_PORT_STATUS0x11/* Status */
#define IGP01E1000_PHY_PORT_CTRL0x12/* Control */
#define IGP01E1000_PHY_LINK_HEALTH0x13/* PHY Link Health */
#define IGP02E1000_PHY_POWER_MGMT0x19/* Power Management */
#define IGP01E1000_PHY_PAGE_SELECT0x1F/* Page Select */
#define BM_PHY_PAGE_SELECT22/* Page Select for BM */
#define IGP_PAGE_SHIFT5
#define PHY_REG_MASK0x1F

/* BM/HV Specific Registers */
#define BM_PORT_CTRL_PAGE769
#define BM_WUC_PAGE800
#define BM_WUC_ADDRESS_OPCODE0x11
#define BM_WUC_DATA_OPCODE0x12
#define BM_WUC_ENABLE_PAGEBM_PORT_CTRL_PAGE
#define BM_WUC_ENABLE_REG17
#define BM_WUC_ENABLE_BITBIT(2)
#define BM_WUC_HOST_WU_BITBIT(4)
#define BM_WUC_ME_WU_BITBIT(5)

#define PHY_UPPER_SHIFT21
#define BM_PHY_REG(page, reg) \
(((reg) & MAX_PHY_REG_ADDRESS) |\
 (((page) & 0xFFFF) << PHY_PAGE_SHIFT) |\
 (((reg) & ~MAX_PHY_REG_ADDRESS) << (PHY_UPPER_SHIFT - PHY_PAGE_SHIFT)))
#define BM_PHY_REG_PAGE(offset) \
((u16)(((offset) >> PHY_PAGE_SHIFT) & 0xFFFF))
#define BM_PHY_REG_NUM(offset) \
((u16)(((offset) & MAX_PHY_REG_ADDRESS) |\
 (((offset) >> (PHY_UPPER_SHIFT - PHY_PAGE_SHIFT)) &\
~MAX_PHY_REG_ADDRESS)))

#define HV_INTC_FC_PAGE_START768
#define I82578_ADDR_REG29
#define I82577_ADDR_REG16
#define I82577_CFG_REG22
#define I82577_CFG_ASSERT_CRS_ON_TXBIT(15)
#define I82577_CFG_ENABLE_DOWNSHIFT(3u << 10)/* auto downshift */
#define I82577_CTRL_REG23

/* 82577 specific PHY registers */
#define I82577_PHY_CTRL_218
#define I82577_PHY_LBK_CTRL19
#define I82577_PHY_STATUS_226
#define I82577_PHY_DIAG_STATUS31

/* I82577 PHY Status 2 */
#define I82577_PHY_STATUS2_REV_POLARITY0x0400
#define I82577_PHY_STATUS2_MDIX0x0800
#define I82577_PHY_STATUS2_SPEED_MASK0x0300
#define I82577_PHY_STATUS2_SPEED_1000MBPS0x0200

/* I82577 PHY Control 2 */
#define I82577_PHY_CTRL2_MANUAL_MDIX0x0200
#define I82577_PHY_CTRL2_AUTO_MDI_MDIX0x0400
#define I82577_PHY_CTRL2_MDIX_CFG_MASK0x0600

/* I82577 PHY Diagnostics Status */
#define I82577_DSTATUS_CABLE_LENGTH0x03FC
#define I82577_DSTATUS_CABLE_LENGTH_SHIFT2

/* BM PHY Copper Specific Control 1 */
#define BM_CS_CTRL116

/* BM PHY Copper Specific Status */
#define BM_CS_STATUS17
#define BM_CS_STATUS_LINK_UP0x0400
#define BM_CS_STATUS_RESOLVED0x0800
#define BM_CS_STATUS_SPEED_MASK0xC000
#define BM_CS_STATUS_SPEED_10000x8000

/* 82577 Mobile Phy Status Register */
#define HV_M_STATUS26
#define HV_M_STATUS_AUTONEG_COMPLETE0x1000
#define HV_M_STATUS_SPEED_MASK0x0300
#define HV_M_STATUS_SPEED_10000x0200
#define HV_M_STATUS_SPEED_1000x0100
#define HV_M_STATUS_LINK_UP0x0040

#define IGP01E1000_PHY_PCS_INIT_REG0x00B4
#define IGP01E1000_PHY_POLARITY_MASK0x0078

#define IGP01E1000_PSCR_AUTO_MDIX0x1000
#define IGP01E1000_PSCR_FORCE_MDI_MDIX0x2000/* 0=MDI, 1=MDIX */

#define IGP01E1000_PSCFR_SMART_SPEED0x0080

#define IGP02E1000_PM_SPD0x0001/* Smart Power Down */
#define IGP02E1000_PM_D0_LPLU0x0002/* For D0a states */
#define IGP02E1000_PM_D3_LPLU0x0004/* For all other states */

#define IGP01E1000_PLHR_SS_DOWNGRADE0x8000

#define IGP01E1000_PSSR_POLARITY_REVERSED0x0002
#define IGP01E1000_PSSR_MDIX0x0800
#define IGP01E1000_PSSR_SPEED_MASK0xC000
#define IGP01E1000_PSSR_SPEED_1000MBPS0xC000

#define IGP02E1000_PHY_CHANNEL_NUM4
#define IGP02E1000_PHY_AGC_A0x11B1
#define IGP02E1000_PHY_AGC_B0x12B1
#define IGP02E1000_PHY_AGC_C0x14B1
#define IGP02E1000_PHY_AGC_D0x18B1

#define IGP02E1000_AGC_LENGTH_SHIFT9/* Course=15:13, Fine=12:9 */
#define IGP02E1000_AGC_LENGTH_MASK0x7F
#define IGP02E1000_AGC_RANGE15

#define E1000_CABLE_LENGTH_UNDEFINED0xFF

#define E1000_KMRNCTRLSTA_OFFSET0x001F0000
#define E1000_KMRNCTRLSTA_OFFSET_SHIFT16
#define E1000_KMRNCTRLSTA_REN0x00200000
#define E1000_KMRNCTRLSTA_CTRL_OFFSET0x1/* Kumeran Control */
#define E1000_KMRNCTRLSTA_DIAG_OFFSET0x3/* Kumeran Diagnostic */
#define E1000_KMRNCTRLSTA_TIMEOUTS0x4/* Kumeran Timeouts */
#define E1000_KMRNCTRLSTA_INBAND_PARAM0x9/* Kumeran InBand Parameters */
#define E1000_KMRNCTRLSTA_IBIST_DISABLE0x0200/* Kumeran IBIST Disable */
#define E1000_KMRNCTRLSTA_DIAG_NELPBK0x1000/* Nearend Loopback mode */
#define E1000_KMRNCTRLSTA_K1_CONFIG0x7
#define E1000_KMRNCTRLSTA_K1_ENABLE0x0002/* enable K1 */
#define E1000_KMRNCTRLSTA_HD_CTRL0x10/* Kumeran HD Control */

#define IFE_PHY_EXTENDED_STATUS_CONTROL0x10
#define IFE_PHY_SPECIAL_CONTROL0x11/* 100BaseTx PHY Special Ctrl */
#define IFE_PHY_SPECIAL_CONTROL_LED0x1B/* PHY Special and LED Ctrl */
#define IFE_PHY_MDIX_CONTROL0x1C/* MDI/MDI-X Control */

/* IFE PHY Extended Status Control */
#define IFE_PESC_POLARITY_REVERSED0x0100

/* IFE PHY Special Control */
#define IFE_PSC_AUTO_POLARITY_DISABLE0x0010
#define IFE_PSC_FORCE_POLARITY0x0020

/* IFE PHY Special Control and LED Control */
#define IFE_PSCL_PROBE_MODE0x0020
#define IFE_PSCL_PROBE_LEDS_OFF0x0006/* Force LEDs 0 and 2 off */
#define IFE_PSCL_PROBE_LEDS_ON0x0007/* Force LEDs 0 and 2 on */

/* IFE PHY MDIX Control */
#define IFE_PMC_MDIX_STATUS0x0020/* 1=MDI-X, 0=MDI */
#define IFE_PMC_FORCE_MDIX0x0040/* 1=force MDI-X, 0=force MDI */
#define IFE_PMC_AUTO_MDIX0x0080/* 1=enable auto, 0=disable */

#endif
