/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 1999 - 2024 Intel Corporation.
 *
 * e1000e-specific kernel version compatibility layer.
 * Generic shims live in ../compat/compat.h; this file adds only
 * definitions that are unique to the e1000e driver.
 */
#ifndef _E1000E_COMPAT_H_
#define _E1000E_COMPAT_H_

#include "../compat/compat.h"

/* struct system_counterval_t switched from 'struct clocksource *cs' to
 * 'enum clocksource_ids cs_id' in Linux 6.9.  On older kernels the caller
 * must use convert_art_to_tsc() to obtain a correctly filled structure.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#define E1000E_SYSVAL_USES_CS_PTR 1
#endif

/* struct cyclecounter's .read callback dropped its 'const' qualifier in 6.13.
 * Use this macro in the function signature so it always matches what the
 * kernel expects on each version.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,13,0)
#define E1000E_CC_READ_CONST const
#else
#define E1000E_CC_READ_CONST
#endif

/* Map the old driver-specific page_pool macro names to the shared ones. */
#define E1000E_PAGE_POOL_NAPI_INIT(napi_ptr)	OOT_PAGE_POOL_NAPI_INIT(napi_ptr)
#define E1000E_PAGE_POOL_SET_NETDEV(params, dev) OOT_PAGE_POOL_SET_NETDEV(params, dev)

#endif /* _E1000E_COMPAT_H_ */
