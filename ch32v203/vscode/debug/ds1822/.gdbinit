#directory src
#target remote :50028
#file build/main.elf
#source lib/gdb-svd.py
#svd lib/CH32V203xx.svd
#monitor reset halt
#load
#break _start
