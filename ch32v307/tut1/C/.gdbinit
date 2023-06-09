directory src
target remote :3333
file build/main.elf
monitor reset halt
#load
#break _start
