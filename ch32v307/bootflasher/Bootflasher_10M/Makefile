# Compiler:
# /opt/ch32_riscv_gcc/bin/riscv-none-embed-gcc --version: === 8.2.0-3.1
# System debugger:
# gdb-multiarch --version:	===	GNU gdb 10.1.90.20210103
# Open On-Chip Debugger:	
# openocd --version === Open On-Chip Debugger 0.11.0 (2021-10-07-10:32)
.PHONY: all clean load erase reset load_gdb
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
		-fno-strict-aliasing \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g3 -O$(OPTIMIZATION) \
		-march=rv32imafc -mabi=ilp32f \
		-fno-pic -fno-common -D"assert_param(x)=" \
		-fno-exceptions -fno-rtti \
		-Wno-pointer-arith \
		-fno-strict-aliasing \
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
$(BLD)main.elf:	$(BLD)core_riscv.o $(BLD)startup_boot.o $(BLD)startup.o 
$(BLD)main.elf: $(BLD)ch32v30x_i2c.o $(BLD)ch32v30x_rcc.o
$(BLD)main.elf: $(BLD)ch32v30x_gpio.o $(BLD)ch32v30x_eth.o $(BLD)ch32v30x_rng.o
$(BLD)main.elf: $(BLD)ch32v30x_misc.o $(BLD)ch32v30x_sdio.o $(BLD)ch32v30x_dma.o
$(BLD)main.elf: $(BLD)memb.o	$(BLD)lwip_list.o
$(BLD)main.elf: $(lwipBLD)ip4.o $(lwipBLD)ip4_frag.o $(lwipBLD)ip4_addr.o
$(BLD)main.elf: $(lwipBLD)icmp.o $(lwipBLD)etharp.o
$(BLD)main.elf: $(lwipBLD)def.o $(lwipBLD)init.o $(lwipBLD)ip.o
$(BLD)main.elf: $(lwipBLD)mem.o $(lwipBLD)memp.o $(lwipBLD)netif.o
$(BLD)main.elf: $(lwipBLD)pbuf.o $(lwipBLD)timeouts.o $(lwipBLD)udp.o
$(BLD)main.elf: $(lwipBLD)inet_chksum.o
$(BLD)main.elf: $(lwipBLD)tcp_out.o $(lwipBLD)tcp_in.o $(lwipBLD)tcp.o
$(BLD)main.elf: $(lwipBLD)ethernet.o $(lwipBLD)ethernetif.o
$(BLD)main.elf: $(BLD)adc.o $(BLD)basic_timers.o $(BLD)dac.o
$(BLD)main.elf: $(BLD)eth.o $(BLD)my_udp.o $(BLD)my_tcp.o
$(BLD)main.elf: $(BLD)flash.o $(BLD)firmware_update.o
$(BLD)main.elf: $(BLD)freq_by_exti.o $(BLD)gptimers.o
$(BLD)main.elf: $(BLD)i2c.o $(BLD)irq.o 
$(BLD)main.elf: $(BLD)memb.o	$(BLD)lwip_list.o
$(BLD)main.elf: $(BLD)protocol.o $(BLD)rcc.o $(BLD)systim.o
$(BLD)main.elf: $(BLD)uart.o
$(BLD)main.elf: $(BLD)main.o
	$(CPP) -o $@ -T$(LIB)Link_boot.ld -T$(LIB)Link.ld $^ -I$(LIB) $(LFLAGS)
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
$(BLD)%.o: $(LIB)%.cpp
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD) $(ffsBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(LIB) $(CPPFLAGS)	
# ASM startup
#$(BLD)startup_ch32v30x_D8.o: $(LIB)startup_ch32v30x_D8.S
#	$(CC) -c $(LIB)startup_ch32v30x_D8.S -o $(BLD)startup_ch32v30x_D8.o 	
#_________________________ LWIP functions  ___________________________________	
$(lwipBLD)%.o: $(lwipSrcCoreIP4)%.c
	$(CPP) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CPPFLAGS)
$(lwipBLD)%.o: $(lwipSrcCore)%.c
	$(CPP) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CPPFLAGS)
$(lwipBLD)%.o: $(lwipSrcNetif)%.cpp
	$(CPP) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CPPFLAGS)

$(lwipBLD)%.o: $(lwip)%.cpp
	$(CPP) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CPPFLAGS)
$(lwipBLD)%.o: $(lwip)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CFLAGS)
$(lwipBLD)%.o: $(lwipSrcApi)%.c
	$(CC) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) \
	$(CFLAGS)
#_____________________________________________________________________________
#_________________________ Basic functions ___________________________________	
#_____________________________________________________________________________
$(BLD)%.o: $(SRC)%.cpp
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) -I$(lwipINCLUDE) -I$(lwip) \
	-I$(freeINC) -I$(ffsINC) $(CPPFLAGS)
$(BLD)%.o: $(perSRC)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) $(CFLAGS)
clean:
	rm -rf build 
	
load:	$(BLD)main.bin
	/opt/openocd/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv.cfg -c init -c halt \
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
