/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * genet-specific kernel version compatibility layer for out-of-tree builds.
 * Generic shims live in ../compat/compat.h; this file adds only
 * definitions that are unique to the genet driver.
 */
#ifndef _GENET_COMPAT_H_
#define _GENET_COMPAT_H_

#include "../compat/compat.h"

/* ────────────────────────────────────────────────────────────────────────────
 * PHY / EEE helpers (pre-6.9)
 * ──────────────────────────────────────────────────────────────────────────── */

/* phy_support_eee() was introduced in 6.9 to advertise EEE capabilities
 * during auto-negotiation.  On older kernels this was handled implicitly
 * by the PHY subsystem; a no-op stub is sufficient.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
static inline void phy_support_eee(struct phy_device *phydev) {}
#endif

/* phydev->enable_tx_lpi was introduced in 6.9 (EEE rework).  On older
 * kernels use eee_enabled which served the same purpose.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#define oot_phydev_tx_lpi(phydev)  ((phydev)->eee_enabled)
#else
#define oot_phydev_tx_lpi(phydev)  ((phydev)->enable_tx_lpi)
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * unimac_mdio_pdata helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* The 'clk' field was added to struct unimac_mdio_pdata in 6.9.
 * On older kernels the clock is managed internally; skip the assignment.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#define OOT_UNIMAC_MDIO_SET_CLK(ppd, clk_ptr)  do { } while (0)
#else
#define OOT_UNIMAC_MDIO_SET_CLK(ppd, clk_ptr)  ((ppd).clk = (clk_ptr))
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * DIM (Dynamic Interrupt Moderation) helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* net_dim() changed signature in 6.18: the end_sample parameter became a
 * pointer (const struct dim_sample *) instead of being passed by value.
 * Wrap the call so the 6.18 calling convention compiles on older kernels.
 */
#include <linux/dim.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(6,18,0)
#define oot_net_dim(dim_ptr, sample_ptr)  net_dim((dim_ptr), *(sample_ptr))
#else
#define oot_net_dim(dim_ptr, sample_ptr)  net_dim((dim_ptr), (sample_ptr))
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * Fixed PHY helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* fixed_phy_register() lost its irq parameter in 6.18.
 * On older kernels pass PHY_POLL as irq to match the new behaviour.
 */
#include <linux/phy_fixed.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(6,18,0)
#define oot_fixed_phy_register(status, np) \
	fixed_phy_register(PHY_POLL, (struct fixed_phy_status *)(status), (np))
#else
#define oot_fixed_phy_register(status, np) \
	fixed_phy_register((status), (np))
#endif

#endif /* _GENET_COMPAT_H_ */
