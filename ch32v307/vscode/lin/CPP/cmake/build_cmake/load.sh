#! /bin/sh
openocd -f ../lib/stlink.cfg -f ../lib/stm32h7x.cfg -c "program \
stm32h743_tcp_udp_build/stm32h743_tcp_udp.bin verify reset exit 0x08000000"
