/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 1999 - 2024 Intel Corporation.
 *
 * Kernel version compatibility layer for the e1000e out-of-tree module.
 * Supports Linux 6.1 through 6.18+.
 */
#ifndef _E1000E_COMPAT_H_
#define _E1000E_COMPAT_H_

#include <linux/version.h>

/* RUNTIME_PM_OPS() was introduced in 6.3.  On 6.1/6.2 only
 * SET_RUNTIME_PM_OPS() is available; define the new macro as a thin
 * wrapper so the rest of the code can use the 6.3+ spelling unchanged.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,3,0)
#define RUNTIME_PM_OPS(suspend_fn, resume_fn, idle_fn) \
	SET_RUNTIME_PM_OPS(suspend_fn, resume_fn, idle_fn)
#endif

/* struct kernel_ethtool_ts_info was split out from struct ethtool_ts_info
 * in 6.8.  On older kernels the ethtool_ops callback takes the plain
 * ethtool_ts_info, so alias the new name to the old one.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,8,0)
#define kernel_ethtool_ts_info ethtool_ts_info
/* struct kernel_hwtstamp_config was introduced in 6.8.  On older kernels
 * hardware timestamping is configured via struct hwtstamp_config through
 * ndo_eth_ioctl; alias the new type to the old one.  Only tx_type and
 * rx_filter are accessed by this driver so the layout difference is safe.
 */
#define kernel_hwtstamp_config hwtstamp_config
#endif

/* kzalloc_objs() was introduced in 6.18.  Provide a simple fallback
 * using kcalloc() for older kernels.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,18,0)
#include <linux/slab.h>
#define kzalloc_objs(TYPE_OR_VAR, COUNT) \
	((typeof(TYPE_OR_VAR) *)kcalloc((COUNT), sizeof(typeof(TYPE_OR_VAR)), GFP_KERNEL))
#endif

#endif /* _E1000E_COMPAT_H_ */
