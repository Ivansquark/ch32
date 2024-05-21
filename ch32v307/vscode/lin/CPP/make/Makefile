#Compiler
#/opt/ch32_riscv_gcc/bin/riscv-none-elf-gcc --version === 12.2.0
# System debugger:
# gdb-multiarch --version:	===	GNU gdb 15.0.50.20240403-git
# Open On-Chip Debugger:	
#/opt/openocd/bin/openocd --version ===
#Open On-Chip Debugger 0.11.0+dev-02415-gfad123a16-dirty (2024-02-19-14:11)

.PHONY: all clean load erase reset load_gdb load_new

#_________________________ GCC compilator commands ____________________________
#GCC_PATH = /opt/ch32_riscv_gcc/bin/riscv-none-embed
GCC_PATH = /opt/ch32_riscv_gcc/bin/riscv-none-elf
CC = $(GCC_PATH)-gcc
CXX = $(GCC_PATH)-g++
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
inc = -I$(INC) -I$(LIB) -I$(perINC) -I$(lwip) -I$(lwipINCLUDE)

#OPTIMIZATION = fast
#OPTIMIZATION = s
#OPTIMIZATION = 1
OPTIMIZATION = 0

CFLAGS = -c -Wall -Wextra -Werror -g -O$(OPTIMIZATION) \
		-march=rv32imafc -mabi=ilp32f \
		-fno-pic -fno-common \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CXXFLAGS = -c -std=c++17 -Wall -Wextra -Werror -g3 -O$(OPTIMIZATION) \
		-march=rv32imafc -mabi=ilp32f \
		-fno-pic -fno-common -D"assert_param(x)=" \
		-fno-exceptions -fno-rtti -fno-threadsafe-statics \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -nostartfiles -march=rv32imafc -mabi=ilp32f \
		 -specs=nano.specs -specs=nosys.specs -fno-exceptions \
		 -fno-use-cxa-atexit \
		 -Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
#-u_printf_float 
#для FPU

#___________________ OBJECTS & DEPENDENCIES ____________________________
CXXSRC = $(lwip)ethernetif.cpp $(wildcard $(SRC)*.cpp) $(wildcard $(LIB)*.cpp)
CXXOBJ := $(addprefix $(BLD),$(CXXSRC:.cpp=.o))
DEPS_CXX = $(CXXOBJ:.o=.d)

CSRC = $(lwipSrcCoreIP4)ip4.c $(lwipSrcCoreIP4)ip4_frag.c \
	$(lwipSrcCoreIP4)ip4_addr.c $(lwipSrcCoreIP4)icmp.c \
	$(lwipSrcCoreIP4)etharp.c $(lwipSrcCore)def.c $(lwipSrcCore)inet_chksum.c \
	$(lwipSrcCore)init.c $(lwipSrcCore)ip.c $(lwipSrcCore)mem.c \
	$(lwipSrcCore)memp.c $(lwipSrcCore)netif.c $(lwipSrcCore)pbuf.c \
	$(lwipSrcCore)timeouts.c $(lwipSrcCore)udp.c $(lwipSrcCore)tcp_out.c \
	$(lwipSrcCore)tcp_in.c $(lwipSrcCore)tcp.c \
	$(lwipSrcNetif)ethernet.c $(lwip)net_led_tmr.c \
	$(wildcard $(perSRC)*.c)
COBJ := $(addprefix $(BLD),$(CSRC:.c=.o))
DEPS_C = $(COBJ:.o=.d)

	
all:	$(BLD)main.bin $(BLD)main.lst $(BLD)main.elf

#--------------------------------- MAKE HEX and BIN ---------------------------
$(BLD)main.bin: $(BLD)main.elf
	$(OBJC) $(BLD)main.elf $(BLD)main.bin -O binary
	$(OBJC) $(BLD)main.elf $(BLD)main.hex -O ihex 
$(BLD)main.lst: $(BLD)main.elf
	$(OBJD) -D $(BLD)main.elf > $(BLD)main.lst

#------------------------------------  LINKER ---------------------------------
$(BLD)main.elf: $(COBJ) $(CXXOBJ)
	$(CC) $^ -o  $@ -T$(LIB)Link.ld $(LFLAGS)
	@echo ''
	@echo "*******************   SIZE   **************************"
	$(GCC_PATH)-size $(BLD)main.elf
	@echo "*******************************************************"
#__________________________________________________________________
#_________________________ BUILD __________________________________	
#__________________________________________________________________

#includes = $(wildcard $(c/inc)/*.h)
$(COBJ) : $(BLD)%.o : %.c 
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(inc)
$(DEPS_C) : $(BLD)%.d : %.c
	mkdir -p $(@D)
	$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) $(inc)  > $@
$(CXXOBJ) : $(BLD)%.o : %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(inc)
$(DEPS_CXX) : $(BLD)%.d : %.cpp
	mkdir -p $(@D)
	$(CXX) -E $(CXXFLAGS) $< -MM -MT $(@:.d=.o) $(inc) > $@

# find headers dependencies
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS_C)
include $(DEPS_CXX)
endif


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

load_new:	$(BLD)main.bin
	/opt/openocd_wch/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv_new.cfg -c init -c halt \
	-c "flash erase_sector wch_riscv 0 last" \
	-c init -c halt -c "program $(BLD)main.bin verify exit"

