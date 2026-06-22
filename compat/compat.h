/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Shared kernel version compatibility layer for out-of-tree network drivers.
 * Supports Linux 6.1 through 6.18+.
 */
#ifndef _OOT_COMPAT_H_
#define _OOT_COMPAT_H_

#include <linux/version.h>

/* ────────────────────────────────────────────────────────────────────────────
 * PM helpers
 * ──────────────────────────────────────────────────────────────────────────── */

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

/* ────────────────────────────────────────────────────────────────────────────
 * Allocator helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* kzalloc_objs() was introduced in 6.18.  Provide a simple fallback
 * using kcalloc() for older kernels.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,18,0)
#include <linux/slab.h>
#define kzalloc_objs(TYPE_OR_VAR, COUNT) \
	((typeof(TYPE_OR_VAR) *)kcalloc((COUNT), sizeof(typeof(TYPE_OR_VAR)), GFP_KERNEL))
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * Timer helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* timer_container_of() was introduced in 6.12 as a replacement for
 * from_timer().  It is a macro so #ifndef is safe.
 */
#ifndef timer_container_of
#define timer_container_of(var, callback_timer, timer_fieldname) \
	from_timer(var, callback_timer, timer_fieldname)
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * NAPI helpers
 * ──────────────────────────────────────────────────────────────────────────── */

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

/* ────────────────────────────────────────────────────────────────────────────
 * Ethtool / hwtstamp helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* struct kernel_ethtool_ts_info was split out from struct ethtool_ts_info
 * in 6.8.  On older kernels the ethtool_ops callback takes the plain
 * ethtool_ts_info, so alias the new name to the old one.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,8,0)
#define kernel_ethtool_ts_info ethtool_ts_info
/* struct kernel_hwtstamp_config was introduced in 6.8.  On older kernels
 * hardware timestamping is configured via struct hwtstamp_config through
 * ndo_eth_ioctl; alias the new type to the old one.  Only tx_type and
 * rx_filter are accessed so the layout difference is safe.
 */
#define kernel_hwtstamp_config hwtstamp_config
#endif

/* ethtool_puts() was introduced in 6.9 as a fixed-width string copy helper.
 * On older kernels forward to ethtool_sprintf() which has the same effect.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#include <linux/ethtool.h>
static inline void ethtool_puts(u8 **data, const char *str)
{
	ethtool_sprintf(data, "%s", str);
}
#endif

/* struct ethtool_keee (EEE with linkmode bitmaps) was introduced in 6.9.
 * On older kernels alias it to the legacy struct ethtool_eee so drivers
 * using the new name compile without changes.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
#define ethtool_keee ethtool_eee
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * PTP / timekeeping helpers
 * ──────────────────────────────────────────────────────────────────────────── */

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

/* ────────────────────────────────────────────────────────────────────────────
 * PHY / EEE helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* rgmii_clock() was introduced in 6.13 to map link speed to RGMII clock
 * rates.  Provide an equivalent for older kernels.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,13,0)
#include <linux/phy.h>
static inline long rgmii_clock(int speed)
{
	switch (speed) {
	case SPEED_10:
		return 2500000;
	case SPEED_100:
		return 25000000;
	case SPEED_1000:
		return 125000000;
	default:
		return -EINVAL;
	}
}
#endif

/* phy_disable_eee_mode() was introduced in 6.18 to clear a single EEE link
 * mode from the PHY's advertised set.  On 6.9..6.17 manipulate
 * advertising_eee directly.  On < 6.9 advertising_eee doesn't exist;
 * provide a no-op since those kernels manage EEE differently.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,9,0)
static inline void phy_disable_eee_mode(struct phy_device *phydev,
					int link_mode)
{
	/* advertising_eee does not exist before 6.9; nothing to do */
}
#elif LINUX_VERSION_CODE < KERNEL_VERSION(6,18,0)
#include <linux/phy.h>
static inline void phy_disable_eee_mode(struct phy_device *phydev,
					int link_mode)
{
	linkmode_clear_bit(link_mode, phydev->advertising_eee);
}
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * Phylink helpers (6.13+ API changes)
 * ──────────────────────────────────────────────────────────────────────────── */

/* pcs_get_state() gained an 'unsigned int neg_mode' parameter in 6.13.
 * Use these macros in function signatures and ops tables so the same
 * source compiles on both old and new kernels.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,13,0)
#define OOT_PCS_GET_STATE_NEG_MODE_ARG	unsigned int neg_mode,
#else
#define OOT_PCS_GET_STATE_NEG_MODE_ARG
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * Page pool helpers
 * ──────────────────────────────────────────────────────────────────────────── */

/* page_pool headers were reorganised in Linux 6.6: the monolithic
 * net/page_pool.h was split into net/page_pool/types.h, core.h and helpers.h.
 * On older kernels include the original combined header.
 *
 * On kernels >= 6.6, net/page_pool/helpers.h transitively pulls in net/xdp.h
 * (and thereby uapi/linux/bpf.h) which defines XDP_PACKET_HEADROOM.  The old
 * net/page_pool.h does not, so provide the fallback define here.
 *
 * Note: page_pool_dma_sync_for_cpu() and page_pool_params.{napi,netdev} were
 * added later (not in all 6.6 RT/stable builds); those are guarded separately
 * below at 6.7.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,6,0)
#include <net/page_pool/helpers.h>
#else
#include <net/page_pool.h>
#ifndef XDP_PACKET_HEADROOM
#define XDP_PACKET_HEADROOM 256
#endif
#endif

/* page_pool_dma_sync_for_cpu() was introduced in Linux 6.7 (not present in
 * all 6.6 stable/RT builds).  Provide an equivalent implementation on older
 * kernels using dma_sync_single_range_for_cpu() directly.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6,7,0)
#include <linux/dma-mapping.h>
static inline void page_pool_dma_sync_for_cpu(const struct page_pool *pool,
					      const struct page *page,
					      u32 offset, u32 dma_sync_size)
{
	dma_sync_single_range_for_cpu(pool->p.dev,
				      page_pool_get_dma_addr((struct page *)page),
				      offset, dma_sync_size,
				      pool->p.dma_dir);
}
#endif

/* struct page_pool_params gained 'napi', 'netdev' and 'queue_idx' members in
 * Linux 6.7 (not reliably present in all 6.6 stable/RT builds).
 * Guard these initializers on older kernels where the fields do not exist.
 *
 * OOT_PAGE_POOL_NAPI_INIT expands to a designated-initializer fragment
 * (including the trailing comma) so it can be used inside a compound literal.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,7,0)
#define OOT_PAGE_POOL_NAPI_INIT(napi_ptr)	.napi = (napi_ptr),
#define OOT_PAGE_POOL_SET_NETDEV(params, dev)	\
	do { (params)->netdev = (dev); } while (0)
#else
#define OOT_PAGE_POOL_NAPI_INIT(napi_ptr)
#define OOT_PAGE_POOL_SET_NETDEV(params, dev)	do { } while (0)
#endif

/* ────────────────────────────────────────────────────────────────────────────
 * platform_driver.remove return type
 * ──────────────────────────────────────────────────────────────────────────── */

/* platform_driver.remove changed its return type from int to void in 6.11.
 * On 6.3–6.10 the void-returning callback belongs in .remove_new; the old
 * .remove field still expects int.  On < 6.3 only .remove (int) exists so
 * we generate a tiny wrapper.
 *
 * Usage in a driver:
 *   OOT_PLATFORM_REMOVE_DECL(my_remove_fn);   // right after the void defn
 *   static struct platform_driver my_drv = {
 *       OOT_PLATFORM_REMOVE(my_remove_fn),    // expands to .remove[_new] = …
 *   };
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6,11,0)
#define OOT_PLATFORM_REMOVE_DECL(fn)
#define OOT_PLATFORM_REMOVE(fn)        .remove = (fn)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(6,3,0)
#define OOT_PLATFORM_REMOVE_DECL(fn)
#define OOT_PLATFORM_REMOVE(fn)        .remove_new = (fn)
#else
#define OOT_PLATFORM_REMOVE_DECL(fn) \
	static int fn##__remove_compat(struct platform_device *pdev) \
	{ fn(pdev); return 0; }
#define OOT_PLATFORM_REMOVE(fn)        .remove = fn##__remove_compat
#endif

#endif /* _OOT_COMPAT_H_ */
