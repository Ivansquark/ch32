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
#///////////////////////////////////
CFLAGS = -c -Wall -Wextra -g -O0 \
	   -fno-pic -fno-common \
	   -Wno-pointer-arith \
	   -ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g -O0 \
	   -fno-pic -fno-common -D"assert_param(x)=" \
	   -fno-exceptions \
	   -Wno-pointer-arith -fno-rtti \
	   -ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -nostartfiles \
		 -specs=nano.specs -specs=nosys.specs -fno-exceptions \
		 -Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
ASFLAGS = -march=rv32imac -mabi=ilp32
#-fno-use-cxa-atexit \
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
$(BLD)main.elf:	$(BLD)startup_as.o
$(BLD)main.elf: $(BLD)main.o
	$(CC) -o $@ -T$(LIB)Link.ld $^ -I$(LIB) $(LFLAGS)
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
$(BLD)startup_as.o: $(LIB)startup_as.S
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(AS) -c $(LIB)startup_as.S -o $(BLD)startup_as.o $(ASFLAGS)
	$(OBJD) -dr -S $(BLD)startup_as.o > $(BLD)startup_as.lst
#_____________________________________________________________________________
#_________________________ Basic functions ___________________________________	
#_____________________________________________________________________________
$(BLD)%.o: $(SRC)%.S
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	#$(CC) -E -c $< -o $@ $(ASFLAGS) -I$(INC) -I$(LIB)
	$(CC) -E -c $(INC)defs.S -o $(BLD)defs.S $(ASFLAGS) -I$(INC) -I$(LIB)
	$(CC) -c $(SRC)main.S -o $(BLD)main.o $(ASFLAGS) -I$(INC)
	$(OBJD) -dr -S $(BLD)main.o > $(BLD)main.lst
clean:
	rm -rf build 
	
load:	$(BLD)main.bin
	/opt/openocd/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv.cfg -c init -c halt \
	-c "flash erase_sector wch_riscv 0 last" \
	-c "wlink_reset_resume" \
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
	gdb-multiarch build/main.elf load q y
	killall -9 openocd
