# Compiler:
# /opt/ch32_riscv_gcc/bin/riscv-none-embed-gcc --version: === 8.2.0-3.1
# System debugger:
# gdb-multiarch --version:	===	GNU gdb 10.1.90.20210103
# Open On-Chip Debugger:	
# openocd --version === Open On-Chip Debugger 0.11.0 (2021-10-07-10:32)
.PHONY: all clean load erase reset load_gdb
#_________________________ GCC compilator commands ____________________________
#GCC_PATH = /opt/ch32_riscv_gcc/bin/riscv-none-embed
GCC_PATH = /opt/ch32_riscv_gcc_12/bin/riscv-none-elf
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
#///////////////////////////////////
inc = -I$(INC) -I$(LIB) -I$(perINC) -I$(lwip) -I$(lwipINCLUDE)
#///////////////////////////////////
#OPTIMIZATION = fast
OPTIMIZATION = s
#OPTIMIZATION = 1
#OPTIMIZATION = 0
#///////////////////////////////////
ARCH_FLAGS = -march=rv32ec -mabi=ilp32e

CFLAGS = -c -Wall -Wextra -g -O$(OPTIMIZATION) ${ARCH_FLAGS} \
		-fno-pic -fno-common \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CXXFLAGS = -c -std=c++17 -Wall -Wextra -g -O$(OPTIMIZATION) ${ARCH_FLAGS} \
		-fno-pic -fno-common -D"assert_param(x)=" \
		-fno-use-cxa-atexit \
		-fno-exceptions -Wno-pointer-arith -fno-rtti \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -lstdc++ -nostartfiles -lm ${ARCH_FLAGS} \
		-specs=nano.specs -specs=nosys.specs \
		-fno-exceptions -fno-use-cxa-atexit \
		-Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
#
#___________________ OBJECTS & DEPENDENCIES ____________________________
CXXSRC = $(wildcard $(SRC)*.cpp) $(wildcard $(LIB)*.cpp)
CXXSRC := $(filter-out src/tirq.cpp, $(CXXSRC))
CXXOBJ := $(addprefix $(BLD),$(CXXSRC:.cpp=.o))
DEPS_CXX = $(CXXOBJ:.o=.d)

CSRC = $(wildcard $(perSRC)*.c)
COBJ := $(addprefix $(BLD),$(CSRC:.c=.o))
DEPS_C = $(COBJ:.o=.d)
	
all:	$(BLD)main.bin $(BLD)main.lst $(BLD)main.elf

#-------------------------------------- HEX -----------------------------------
$(BLD)main.bin: $(BLD)main.elf
	$(OBJC) $(BLD)main.elf $(BLD)main.bin -O binary
	$(OBJC) $(BLD)main.elf $(BLD)main.hex -O ihex 
$(BLD)main.lst: $(BLD)main.elf
	$(OBJD) -D $(BLD)main.elf > $(BLD)main.lst

#------------------------------------  LINKER ---------------------------------
$(BLD)main.elf: $(COBJ) $(CXXOBJ)
	$(CC) $^ -o  $@ -T$(LIB)Link.ld $(LFLAGS)
#    $@ — target name
#    $< — first dependancy name
#    $? — all dependancies names, thats newer than target
#    $^ — all dependencies names of the target
#	 %	- template for all files
#-------------------------------------- SIZE ----------------------------------	
	@echo "************************************************************"
	@echo "*******************   SIZE   *******************************"
	@echo "************************************************************"
	$(GCC_PATH)-size $(BLD)main.elf
	@echo "************************************************************"

#___________________ BUILD __________________________________	
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
	/opt/openocd_wch/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv_new.cfg -c init -c halt \
	-c "flash erase_sector wch_riscv 0 last" \
	-c init -c halt -c "program $(BLD)main.bin verify exit"

#load:	$(BLD)main.bin
#	/opt/openocd/bin/openocd -s /usr/local/share/openocd/scripts \
#	-f lib/wch-riscv.cfg -c init -c halt \
#	-c "flash erase_sector wch_riscv 0 last" \
#	-c "wlink_reset_resume" \
#	-c init -c halt -c "program $(BLD)main.bin verify exit"
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
	gdb-multiarch build/main.elf load q y
	killall -9 openocd
