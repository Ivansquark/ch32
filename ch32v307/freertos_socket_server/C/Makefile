# Compiler:
# /opt/ch32_riscv_gcc/bin/riscv-none-embed-gcc --version: === 8.2.0-3.1
# System debugger:
# gdb-multiarch --version:	===	GNU gdb 10.1.90.20210103
# Open On-Chip Debugger:	
# openocd --version === Open On-Chip Debugger 0.11.0 (2021-10-07-10:32)
.PHONY: all clean load load_new erase reset load_gdb
#_________________________ GCC compilator commands ____________________________
GCC_PATH = /opt/ch32_riscv_gcc/bin/riscv-none-embed
CC = $(GCC_PATH)-gcc
CPP = $(GCC_PATH)-g++
OBJC = $(GCC_PATH)-objcopy
OBJD = $(GCC_PATH)-objdump
AS = $(GCC_PATH)-as
LD = $(GCC_PATH)-ld

INC = inc/
LIB = lib/
BLD = build/
SRC = src/
perINC = Peripheral/inc/
perSRC = Peripheral/src/
#////////////// FreeRTOS ///////////
freeINC = FreeRTOS/include/
freeSRC = FreeRTOS/
#////////////// FFS ////////////////
ffsINC = ffs/inc/
ffsSRC = ffs/src/
ffsBLD = build/ffs/
#///////////////////////////////////
lwipINCLUDE = LWIP/include/
lwip = LWIP/
#lwipArch = LWIP/arch/
lwipSrcApi = LWIP/api/
lwipSrcCore = LWIP/core/
lwipSrcCoreIP4 = LWIP/core/ipv4/
lwipSrcNetif = LWIP/netif/
#lwipSrcNetifPPP = LWIP/netif/ppp/
lwipBLD = build/lwip/
#lwipSYSTEM = LWIP/system/
#///////////////////////////////////
#OPTIMIZATION = fast
OPTIMIZATION = s
#OPTIMIZATION = 0


CFLAGS = -c -Wall -Wextra -g -O$(OPTIMIZATION) \
		-march=rv32imafc -mabi=ilp32f \
		-fno-pic -fno-common \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g3 -O$(OPTIMIZATION) \
		-march=rv32imafc -mabi=ilp32f \
		-fno-pic -fno-common -D"assert_param(x)=" \
		-fno-exceptions -fno-rtti \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -nostartfiles -march=rv32imafc -mabi=ilp32f \
		 -specs=nano.specs -specs=nosys.specs -fno-exceptions \
		 -fno-use-cxa-atexit \
		 -Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
#-u_printf_float 
#для FPU
	
all:	$(BLD)main.bin $(BLD)main.lst $(BLD)main.elf

#-------------------------------------- HEX -----------------------------------
$(BLD)main.bin: $(BLD)main.elf
	$(OBJC) $(BLD)main.elf $(BLD)main.bin -O binary
	$(OBJC) $(BLD)main.elf $(BLD)main.hex -O ihex 
$(BLD)main.lst: $(BLD)main.elf
	$(OBJD) -D $(BLD)main.elf > $(BLD)main.lst

#------------------------------------  LINKER ---------------------------------
$(BLD)main.elf:	$(BLD)core_riscv.o $(BLD)startup.o
$(BLD)main.elf: $(BLD)ch32v30x_i2c.o $(BLD)ch32v30x_spi.o $(BLD)ch32v30x_rcc.o
$(BLD)main.elf: $(BLD)ch32v30x_gpio.o $(BLD)ch32v30x_eth.o $(BLD)ch32v30x_rng.o
$(BLD)main.elf: $(BLD)ch32v30x_misc.o $(BLD)ch32v30x_sdio.o $(BLD)ch32v30x_dma.o
$(BLD)main.elf: $(BLD)memb.o	$(BLD)lwip_list.o
$(BLD)main.elf: $(lwipBLD)ip4.o $(lwipBLD)ip4_frag.o $(lwipBLD)ip4_addr.o
$(BLD)main.elf: $(lwipBLD)icmp.o $(lwipBLD)etharp.o
$(BLD)main.elf: $(lwipBLD)sockets.o $(lwipBLD)err.o $(lwipBLD)tcpip.o 
#$(BLD)main.elf: $(lwipBLD)api_msg.o $(lwipBLD)api_lib.o 
$(BLD)main.elf: $(lwipBLD)def.o $(lwipBLD)init.o $(lwipBLD)ip.o
$(BLD)main.elf: $(lwipBLD)mem.o $(lwipBLD)memp.o $(lwipBLD)netif.o
$(BLD)main.elf: $(lwipBLD)pbuf.o $(lwipBLD)timeouts.o $(lwipBLD)udp.o
$(BLD)main.elf: $(lwipBLD)inet_chksum.o
$(BLD)main.elf: $(lwipBLD)tcp_out.o $(lwipBLD)tcp_in.o $(lwipBLD)tcp.o
$(BLD)main.elf: $(lwipBLD)ethernet.o $(lwipBLD)ethernetif.o
$(BLD)main.elf: $(lwipBLD)ethernet.o $(lwipBLD)ethernetif.o
$(BLD)main.elf: $(lwipBLD)sys_arch.o
$(BLD)main.elf: $(BLD)eth.o #$(BLD)my_udp.o $(BLD)my_tcp.o $(BLD)my_http.o
$(BLD)main.elf: $(BLD)rcc.o $(BLD)eeprom.o $(BLD)gpio.o $(BLD)w25q.o
$(BLD)main.elf: $(BLD)basic_timer.o
$(BLD)main.elf: $(BLD)adc.o
$(BLD)main.elf: $(BLD)main.o
$(BLD)main.elf: $(BLD)portASM.o
$(BLD)main.elf: $(BLD)tasks.o $(BLD)list.o $(BLD)port.o $(BLD)heap_4.o
$(BLD)main.elf: $(BLD)queue.o
	$(CPP) -o $@ -T$(LIB)Link.ld $^ -I$(LIB) $(LFLAGS)
#    $@ — target name
#    $< — first dependancy name
#    $? — all dependancies names, thats newer than target
#    $^ — all dependencies names of the target
#	 %	- template for all files

#-------------------------------------- SIZE ----------------------------------	
	@echo "*******************************************************"
	@echo "*******************   SIZE   **************************"
	@echo "*******************************************************"
	$(GCC_PATH)-size $(BLD)main.elf
	@echo "*******************************************************"
#___________________ BUILD __________________________________	
$(BLD)%.o: $(LIB)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD) $(ffsBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) $(CFLAGS)
# ASM startup
#$(BLD)startup_ch32v30x_D8.o: $(LIB)startup_ch32v30x_D8.S
#	$(CC) -c $(LIB)startup_ch32v30x_D8.S -o $(BLD)startup_ch32v30x_D8.o 	
#_________________________ LWIP functions  ___________________________________	
$(lwipBLD)%.o: $(lwipSrcCoreIP4)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CFLAGS)
$(lwipBLD)%.o: $(lwipSrcCore)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	-I$(freeINC) $(CFLAGS)
$(lwipBLD)%.o: $(lwipSrcNetif)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CFLAGS)

$(lwipBLD)%.o: $(lwip)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CFLAGS)
$(lwipBLD)%.o: $(lwip)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	-I$(freeINC) $(CFLAGS)
$(lwipBLD)%.o: $(lwipSrcApi)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	-I$(freeINC) $(CFLAGS)
#_____________________________________________________________________________
#_________________________ FreeRTOS __________________________________________	
#_____________________________________________________________________________
# ASM startup
$(BLD)portASM.o: $(freeSRC)portASM.S
	$(CPP) -c $(freeSRC)portASM.S -o $(BLD)portASM.o -I$(freeINC) $(CPPFLAGS)
$(BLD)%.o: $(freeSRC)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) -I$(freeINC) $(CFLAGS)
#_____________________________________________________________________________
#_________________________ Basic functions ___________________________________	
#_____________________________________________________________________________
$(BLD)%.o: $(SRC)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD) $(ffsBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) -I$(lwipINCLUDE) -I$(lwip) \
	-I$(freeINC) -I$(ffsINC) $(CFLAGS)
$(BLD)%.o: $(perSRC)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD) $(ffsBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC)  -I$(freeINC) $(CFLAGS)
clean:
	rm -rf build 
	
load:	$(BLD)main.bin
	/opt/openocd/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv.cfg -c init -c halt \
	-c "flash erase_sector wch_riscv 0 last" \
	-c init -c halt -c "program $(BLD)main.bin verify exit"
load_new:	$(BLD)main.bin
	/opt/openocd_wch/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv_new.cfg -c init -c halt \
	-c "flash erase_sector wch_riscv 0 last" \
	-c init -c halt -c "program $(BLD)main.bin verify exit"
# must be boot0 to map flash from 0 to 0x08000000

erase:
	/opt/openocd/bin/openocd -s /opt/openocd/share/openocd/scripts \
	   -f lib/wch-riscv.cfg -c init -c halt \
	   -c "flash erase_sector wch_riscv 0 last" -c exit
reset:
	/opt/openocd/bin/openocd -s /opt/openocd/share/openocd/scripts \
	-f lib/wch-riscv.cfg -c init -c halt -c wlink_reset_resume -c exit

load_gdb:
	/opt/openocd/bin/openocd -s /opt/openocd/share/openocd/scripts \
	   -f lib/wch-riscv.cfg -c init -c halt \
	   -c "flash erase_sector wch_riscv 0 last" -c exit
	/opt/openocd/bin/openocd -s /opt/openocd/share/openocd/scripts \
	-f lib/wch-riscv.cfg &
	gdb-multiarch build/main.elf load
	killall -9 openocd
