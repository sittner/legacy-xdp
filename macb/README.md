# macb-xdp

Out-of-tree Cadence MACB/GEM Ethernet driver with native XDP support.

## What's changed

Based on the upstream driver from
[raspberrypi/linux](https://github.com/raspberrypi/linux) `rpi-6.18.y`
(`drivers/net/ethernet/cadence/`), with these additions:

- **Page pool RX** — `gem_rx()`/`gem_rx_refill()` use `page_pool` instead of
  `netdev_alloc_skb()`, enabling zero-copy XDP buffer management.
- **XDP core** — `.ndo_bpf` program attach/detach, RX verdict handling
  (`XDP_PASS`, `XDP_DROP`, `XDP_TX`, `XDP_ABORTED`).
- **XDP redirect** — `.ndo_xdp_xmit` for `XDP_REDIRECT` from other devices
  and cross-device forwarding.
- **e1000e parity** — the sibling `e1000e/` out-of-tree driver now also has
  native XDP wiring (`ndo_bpf`, RX XDP execution, `XDP_TX`/`XDP_REDIRECT`,
  and `xdp_rxq_info` registration).

## Build

Requires kernel headers matching the running kernel.

```bash
make
sudo insmod macb_xdp.ko    # or replace the in-tree module
```

## Usage

```bash
# Attach an XDP program
ip link set dev eth0 xdp obj xdp_pass.o sec xdp

# Detach
ip link set dev eth0 xdp off
```

Works with EtherCAT userspace masters using `xdp-native` transport.

## Tested on

- Raspberry Pi (aarch64), kernel 6.18.35-v8-16k-rt
- 3 EtherCAT slaves via native XDP transport
