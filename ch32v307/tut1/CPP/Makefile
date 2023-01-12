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
CFLAGS = -c -Wall -Wextra -g -O0 \
	   -fno-pic -fno-common \
	   -Wno-pointer-arith \
	   -ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g3 -Ofast \
	   -fno-pic -fno-common -D"assert_param(x)=" \
	   -fno-exceptions \
	   -Wno-pointer-arith -fno-rtti \
	   -ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -nostartfiles \
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
$(BLD)main.elf: $(BLD)rcc.o $(BLD)eeprom.o $(BLD)gpio.o $(BLD)w25q.o
$(BLD)main.elf: $(BLD)basic_timer.o $(BLD)systim.o $(BLD)eth.o
$(BLD)main.elf: $(BLD)memb.o	$(BLD)list.o
$(BLD)main.elf: $(lwipBLD)ip4.o $(lwipBLD)ip4_frag.o $(lwipBLD)ip4_addr.o
$(BLD)main.elf: $(lwipBLD)icmp.o $(lwipBLD)etharp.o
$(BLD)main.elf: $(lwipBLD)def.o $(lwipBLD)init.o $(lwipBLD)ip.o
$(BLD)main.elf: $(lwipBLD)mem.o $(lwipBLD)memp.o $(lwipBLD)netif.o
$(BLD)main.elf: $(lwipBLD)pbuf.o $(lwipBLD)timeouts.o $(lwipBLD)udp.o
$(BLD)main.elf: $(lwipBLD)inet_chksum.o
$(BLD)main.elf: $(lwipBLD)tcp_out.o $(lwipBLD)tcp_in.o $(lwipBLD)tcp.o
$(BLD)main.elf: $(lwipBLD)ethernet.o $(lwipBLD)ethernetif.o
$(BLD)main.elf: $(BLD)eth.o $(BLD)my_udp.o $(BLD)my_tcp.o $(BLD)my_http.o
$(BLD)main.elf: $(BLD)main.o
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
	arm-none-eabi-size $(BLD)main.elf
	@echo "*******************************************************"
#___________________ BUILD __________________________________	
$(BLD)%.o: $(LIB)%.cpp
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(LIB) $(CPPFLAGS)	
# ASM startup
#$(BLD)startup_ch32v30x_D8.o: $(LIB)startup_ch32v30x_D8.S
#	$(CC) -c $(LIB)startup_ch32v30x_D8.S -o $(BLD)startup_ch32v30x_D8.o 	
#_____________________________________________________________________________
#_________________________ LWIP functions  ___________________________________	
#_____________________________________________________________________________
$(lwipBLD)ip4.o: $(lwipSrcCoreIP4)ip4.cpp
	$(CC) $(lwipSrcCoreIP4)ip4.cpp -o $(lwipBLD)ip4.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)ip4_frag.o: $(lwipSrcCoreIP4)ip4_frag.cpp
	$(CC) $(lwipSrcCoreIP4)ip4_frag.cpp -o $(lwipBLD)ip4_frag.o -I$(INC) \
	-I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)ip4_addr.o: $(lwipSrcCoreIP4)ip4_addr.cpp
	$(CC) $(lwipSrcCoreIP4)ip4_addr.cpp -o $(lwipBLD)ip4_addr.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)icmp.o: $(lwipSrcCoreIP4)icmp.cpp
	$(CC) $(lwipSrcCoreIP4)icmp.cpp -o $(lwipBLD)icmp.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)etharp.o: $(lwipSrcCoreIP4)etharp.cpp
	$(CC) $(lwipSrcCoreIP4)etharp.cpp -o $(lwipBLD)etharp.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(lwipBLD)def.o: $(lwipSrcCore)def.cpp
	$(CC) $(lwipSrcCore)def.cpp -o $(lwipBLD)def.o -I$(INC) \
	-I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)init.o: $(lwipSrcCore)init.cpp
	$(CC) $(lwipSrcCore)init.cpp -o $(lwipBLD)init.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)ip.o: $(lwipSrcCore)ip.cpp
	$(CC) $(lwipSrcCore)ip.cpp -o $(lwipBLD)ip.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)mem.o: $(lwipSrcCore)mem.cpp
	$(CC) $(lwipSrcCore)mem.cpp -o $(lwipBLD)mem.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)memp.o: $(lwipSrcCore)memp.cpp
	$(CC) $(lwipSrcCore)memp.cpp -o $(lwipBLD)memp.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)netif.o: $(lwipSrcCore)netif.cpp
	$(CC) $(lwipSrcCore)netif.cpp -o $(lwipBLD)netif.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)pbuf.o: $(lwipSrcCore)pbuf.cpp
	$(CC) $(lwipSrcCore)pbuf.cpp -o $(lwipBLD)pbuf.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)timeouts.o: $(lwipSrcCore)timeouts.cpp
	$(CC) $(lwipSrcCore)timeouts.cpp -o $(lwipBLD)timeouts.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)inet_chksum.o: $(lwipSrcCore)inet_chksum.cpp
	$(CC) $(lwipSrcCore)inet_chksum.cpp -o $(lwipBLD)inet_chksum.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(lwipBLD)udp.o: $(lwipSrcCore)udp.cpp
	$(CC) $(lwipSrcCore)udp.cpp -o $(lwipBLD)udp.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(lwipBLD)tcp.o: $(lwipSrcCore)tcp.cpp
	$(CC) $(lwipSrcCore)tcp.cpp -o $(lwipBLD)tcp.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)tcp_in.o: $(lwipSrcCore)tcp_in.cpp
	$(CC) $(lwipSrcCore)tcp_in.cpp -o $(lwipBLD)tcp_in.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)
$(lwipBLD)tcp_out.o: $(lwipSrcCore)tcp_out.cpp
	$(CC) $(lwipSrcCore)tcp_out.cpp -o $(lwipBLD)tcp_out.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(lwipBLD)ethernet.o: $(lwipSrcNetif)ethernet.cpp
	$(CC) $(lwipSrcNetif)ethernet.cpp -o $(lwipBLD)ethernet.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(lwipBLD)ethernetif.o: $(lwip)ethernetif.cpp
	$(CC) $(lwip)ethernetif.cpp -o $(lwipBLD)ethernetif.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

$(BLD)eth.o: $(SRC)eth.cpp
	$(CC) $(SRC)eth.cpp -o $(BLD)eth.o -I$(INC) \
	-I$(perINC) -I$(LIB) -I$(lwipINCLUDE) -I$(lwip) $(CPPFLAGS)

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
	$(CPPFLAGS)
$(BLD)%.o: $(perSRC)%.cpp
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) $(CPPFLAGS)
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
