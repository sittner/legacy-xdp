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
 * Some vendor kernels (e.g. Debian 6.1.0-49) backport the macro, so
 * guard with #ifndef rather than a version check.
 */
#ifndef RUNTIME_PM_OPS
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

/* timer_container_of() was introduced in 6.12 as a replacement for
 * from_timer().  It is a macro so #ifndef is safe.
 */
#ifndef timer_container_of
#define timer_container_of(var, callback_timer, timer_fieldname) \
	from_timer(var, callback_timer, timer_fieldname)
#endif

/* netif_napi_set_irq() was introduced in 6.5 to associate a NAPI instance
 * with a specific IRQ for affinity hinting.  It is a no-op on older kernels.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,5,0)
static inline void netif_napi_set_irq(struct napi_struct *napi, int irq) {}
#endif

/* netif_queue_set_napi() and the netdev_queue_type enum were introduced in
 * 6.6.  Provide empty stubs on older kernels.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,6,0)
enum netdev_queue_type {
	NETDEV_QUEUE_TYPE_RX,
	NETDEV_QUEUE_TYPE_TX,
};
static inline void netif_queue_set_napi(struct net_device *dev,
					unsigned int queue_index,
					enum netdev_queue_type type,
					struct napi_struct *napi) {}
#endif

/* diff_by_scaled_ppm() and adjust_by_scaled_ppm() were introduced in 6.2.
 * Provide equivalent implementations using mul_u64_u64_div_u64, which has
 * been available since before 6.1.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,2,0)
#include <linux/math64.h>
static inline bool diff_by_scaled_ppm(u64 base, long scaled_ppm, u64 *diff)
{
	bool negative = false;

	if (scaled_ppm < 0) {
		negative = true;
		scaled_ppm = -scaled_ppm;
	}
	*diff = mul_u64_u64_div_u64(base, (u64)scaled_ppm, 1000000ULL << 16);
	return negative;
}

static inline u64 adjust_by_scaled_ppm(u64 base, long scaled_ppm)
{
	u64 diff;

	if (diff_by_scaled_ppm(base, scaled_ppm, &diff))
		return base - diff;
	return base + diff;
}
#endif

/* struct system_counterval_t switched from 'struct clocksource *cs' to
 * 'enum clocksource_ids cs_id' in Linux 6.9.  On older kernels the caller
 * must use convert_art_to_tsc() to obtain a correctly filled structure.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#define E1000E_SYSVAL_USES_CS_PTR 1
#endif

/* struct cyclecounter's .read callback dropped its 'const' qualifier in 6.12.
 * Use this macro in the function signature so it always matches what the
 * kernel expects on each version.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,12,0)
#define E1000E_CC_READ_CONST const
#else
#define E1000E_CC_READ_CONST
#endif

/* page_pool headers were reorganised in Linux 6.6: the monolithic
 * net/page_pool.h was split into net/page_pool/types.h, core.h and helpers.h.
 * On older kernels include the original combined header.
 *
 * On kernels >= 6.6, net/page_pool/helpers.h transitively pulls in net/xdp.h
 * (and thereby uapi/linux/bpf.h) which defines XDP_PACKET_HEADROOM.  The old
 * net/page_pool.h does not, so provide the fallback define here.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
#include <net/page_pool/helpers.h>
#else
#include <net/page_pool.h>
#ifndef XDP_PACKET_HEADROOM
#define XDP_PACKET_HEADROOM 256
#endif
#endif

/* page_pool_dma_sync_for_cpu() was introduced in Linux 6.6 (as part of the
 * helpers.h split).  Provide an equivalent implementation on older kernels
 * using dma_sync_single_range_for_cpu() directly.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,6,0)
#include <linux/dma-mapping.h>
static inline void page_pool_dma_sync_for_cpu(const struct page_pool *pool,
					      const struct page *page,
					      u32 offset, u32 dma_sync_size)
{
	dma_sync_single_range_for_cpu(pool->p.dev,
				      page_pool_get_dma_addr(page),
				      offset, dma_sync_size,
				      pool->p.dma_dir);
}
#endif

/* struct page_pool_params gained 'napi', 'netdev' and 'queue_idx' members in
 * Linux 6.6 (as part of the struct_group_tagged fast/slow reorganisation).
 * Guard these initializers on older kernels where the fields do not exist.
 *
 * E1000E_PAGE_POOL_NAPI_INIT expands to a designated-initializer fragment
 * (including the trailing comma) so it can be used inside a compound literal.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
#define E1000E_PAGE_POOL_NAPI_INIT(napi_ptr)	.napi = (napi_ptr),
#define E1000E_PAGE_POOL_SET_NETDEV(params, dev)	\
	do { (params)->netdev = (dev); } while (0)
#else
#define E1000E_PAGE_POOL_NAPI_INIT(napi_ptr)
#define E1000E_PAGE_POOL_SET_NETDEV(params, dev)	do { } while (0)
#endif

#endif /* _E1000E_COMPAT_H_ */
