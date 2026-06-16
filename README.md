# macb-xdp

Out-of-tree build of the Cadence MACB/GEM Ethernet driver, used as a baseline
for adding native XDP support.

## Source

The driver source files (`macb_main.c`, `macb.h`, `macb_ptp.c`) are copied
verbatim from the
[raspberrypi/linux](https://github.com/raspberrypi/linux) repository,
branch `rpi-6.18.y` (`drivers/net/ethernet/cadence/`).

This is the unmodified upstream driver and serves as the clean starting point
for the XDP development work.

## Build

You need a pre-built kernel source tree that matches the running kernel.
On Raspberry Pi OS the headers are typically installed under
`/lib/modules/$(uname -r)/build`.

```bash
# Build against the running kernel (default)
make

# Build against a specific kernel tree
make KDIR=/path/to/kernel/build

# Clean
make clean
```

The build produces `macb.ko`.

## Goal

Add native XDP support to the macb/GEM driver so that eBPF programs can run
directly in the driver's RX hot path (before `sk_buff` allocation), enabling
high-performance packet processing on Raspberry Pi and similar platforms.
