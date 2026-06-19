# legacy-xdp

Out-of-tree Linux NIC drivers adding native XDP support to legacy hardware
commonly used for EtherCAT.

## Motivation

Userspace EtherCAT masters (such as [sittner/ethercat](https://github.com/sittner/ethercat))
benefit greatly from XDP: eBPF programs run directly in the driver's RX hot path
— before any `sk_buff` allocation — enabling sub-microsecond packet steering
with zero kernel-network-stack overhead.

Many NICs commonly used for EtherCAT do **not** support XDP upstream and likely
never will. This project provides out-of-tree driver builds that retrofit native
XDP plumbing onto those legacy devices, targeting Linux 6.1+ (including
PREEMPT_RT kernels).

## Supported Drivers

| Directory | NIC | Upstream source | Status |
|-----------|-----|-----------------|--------|
| `macb/` | Cadence MACB/GEM (Raspberry Pi, Zynq, …) | [raspberrypi/linux](https://github.com/raspberrypi/linux) `rpi-6.18.y` | ✅ Working |
| `e1000e/` | Intel PRO/1000 (I217/I218/I219, 82574, …) | [torvalds/linux](https://github.com/torvalds/linux) v6.18 | ✅ Working |
| `genet/` | Broadcom GENET (Raspberry Pi 3/4, …) | [raspberrypi/linux](https://github.com/raspberrypi/linux) `rpi-6.18.y` | ✅ Working |
| `r8169/` | Realtek RTL8111/8168/8125 | [torvalds/linux](https://github.com/torvalds/linux) v6.18 | 🧪 Testing |

## Features

- **Page pool RX** — zero-copy buffer management via `page_pool`.
- **XDP core** — `.ndo_bpf` program attach/detach, RX verdict handling
  (`XDP_PASS`, `XDP_DROP`, `XDP_TX`, `XDP_REDIRECT`, `XDP_ABORTED`).
- **XDP redirect** — `.ndo_xdp_xmit` for cross-device forwarding.
- **Compat layer** — builds against kernel headers from Linux 6.1 through 6.18+,
  including PREEMPT_RT variants.

## Build

Requires kernel headers matching the running kernel.

```bash
# Build all drivers
make

# Build a single driver
make -C macb/
make -C e1000e/
make -C genet/
make -C r8169/

# Install (replace in-tree modules)
sudo insmod macb/macb_xdp.ko
sudo insmod e1000e/e1000e_xdp.ko
sudo insmod genet/genet_xdp.ko
sudo insmod r8169/r8169_xdp.ko

# Clean
make clean
```

## DKMS Packages

Each driver can be packaged as a Debian DKMS package that auto-rebuilds the
module on kernel upgrades.

```bash
# Build all .deb packages
make deb

# Install a package
sudo dpkg -i macb-xdp-dkms_*.deb
sudo dpkg -i e1000e-xdp-dkms_*.deb
sudo dpkg -i genet-xdp-dkms_*.deb
sudo dpkg -i r8169-xdp-dkms_*.deb

# Remove a package
sudo dpkg -r macb-xdp-dkms
```

## Usage

```bash
# Attach an XDP program
ip link set dev eth0 xdp obj xdp_pass.o sec xdp

# Detach
ip link set dev eth0 xdp off
```

Works with the [sittner/ethercat](https://github.com/sittner/ethercat) userspace
EtherCAT master using the `xdp-native` transport.

## Tested on

- Raspberry Pi 4/5 (aarch64), kernel 6.1-rt / 6.18-rt
- Intel I219-LM (x86_64), kernel 6.1-rt
- 3 EtherCAT slaves via native XDP transport

## Adding more drivers

Contributions welcome! The general pattern is:

1. Copy the upstream driver source into a new subdirectory.
2. Add a `compat.h` shim for kernel-version differences.
3. Wire up `page_pool` RX, `ndo_bpf`, and `ndo_xdp_xmit`.
4. Add a `Makefile`/`Kbuild` for out-of-tree building.

## License

Each driver subdirectory retains its original upstream license (GPL-2.0).
