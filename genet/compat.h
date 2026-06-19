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
