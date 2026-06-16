savedcmd_macb.mod := printf '%s\n'   macb_main.o macb_ptp.o | awk '!x[$$0]++ { print("./"$$0) }' > macb.mod
