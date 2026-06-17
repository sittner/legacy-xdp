# e1000e — Intel PRO/1000 PCIe Ethernet Driver (out-of-tree)

This directory contains the **upstream Intel e1000e Ethernet driver** extracted
from the Linux 6.15 kernel (latest stable at time of extraction; targeting
compatibility with Linux 6.1 through 6.18+), set up as a **standalone
out-of-tree kernel module**.

The driver source is taken verbatim from:
```
drivers/net/ethernet/intel/e1000e/
```
in the Linux kernel tree (`torvalds/linux`, tag `v6.15`).

## Purpose

This out-of-tree driver now includes **native XDP support** for kernels
6.1 through 6.18+, following the same approach used in the sibling `macb-xdp`
driver in the parent directory.

### Why native XDP for e1000e?

For EtherCAT (and similar real-time Ethernet applications), the key advantage
of native XDP over the generic `xdp-skb` mode is **zero allocation in the
hot path**:

- **RX**: With native XDP + page_pool, no `netdev_alloc_skb()` is called per
  frame. Pages are pre-allocated from the page_pool at ring fill time and
  recycled via XDP verdicts (`XDP_REDIRECT` to AF_XDP socket).
- **TX**: With native XDP, no `sock_alloc_send_skb()` / `kfree_skb()` per
  frame. The UMEM descriptor goes directly to the HW TX ring.

Both paths become **alloc-free and deterministic** in the hot path —
eliminating SLUB allocator overhead and jitter from the EtherCAT cycle.

## Build Instructions

```sh
cd e1000e/
make
```

To build against a specific kernel version:

```sh
make KDIR=/lib/modules/<version>/build
```

To clean:

```sh
make clean
```

The built module is `e1000e.ko`. Load it with:

```sh
sudo insmod e1000e.ko
# or
sudo modprobe e1000e
```

> **Note**: This out-of-tree module replaces the in-tree `e1000e` driver.
> Unload the in-tree module first if it is loaded:
> ```sh
> sudo rmmod e1000e
> ```

## Target Kernel Range

**Linux 6.1 – 6.18+**

The driver source is from v6.15, with local XDP and compat-layer updates
to handle API differences across the 6.1–6.18 range.

## Source Files

All files are copied verbatim from upstream Linux v6.15:

| File | Description |
|------|-------------|
| `netdev.c` | Main driver: probe/remove, RX/TX paths, NAPI |
| `ethtool.c` | ethtool interface |
| `ich8lan.c` | ICH8/ICH9/ICH10/PCH hardware variants |
| `80003es2lan.c` | 80003ES2LAN hardware variant |
| `82571.c` | 82571/82572/82573/82574/82583 hardware variants |
| `mac.c` | MAC operations |
| `manage.c` | Management (host interface) |
| `nvm.c` | EEPROM/NVM operations |
| `phy.c` | PHY operations |
| `param.c` | Module parameters |
| `ptp.c` | PTP hardware clock (PHC) |
| `e1000.h` | Main adapter header |
| `hw.h` | Hardware abstraction layer |
| `defines.h` | Hardware register bit definitions |
| `regs.h` | Hardware register offsets |
| `ich8lan.h` | ICH8LAN-specific definitions |
| `82571.h` | 82571-specific definitions |
| `80003es2lan.h` | 80003ES2LAN-specific definitions |
| `mac.h` | MAC operation declarations |
| `manage.h` | Management operation declarations |
| `nvm.h` | NVM operation declarations |
| `phy.h` | PHY operation declarations |
| `ptp.h` | PTP declarations |
| `e1000e_trace.h` | Tracepoint definitions |

## Reference

- Parent directory: macb-xdp out-of-tree module (same build pattern)
- Upstream reference: `drivers/net/ethernet/intel/e1000e/` in `torvalds/linux`
- EtherCAT master with XDP transport: `sittner/ethercat`
