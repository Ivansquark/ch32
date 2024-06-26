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
#OPTIMIZATION = fast
#OPTIMIZATION = s
#OPTIMIZATION = 1
OPTIMIZATION = 0
#///////////////////////////////////
CFLAGS = -c -Wall -Wextra -g -O$(OPTIMIZATION) -march=rv32ec -mabi=ilp32e \
		-fno-pic -fno-common \
		-Wno-pointer-arith \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g -O$(OPTIMIZATION) -march=rv32ec \
		-mabi=ilp32e -fno-pic -fno-common -D"assert_param(x)=" \
		-fno-exceptions -Wno-pointer-arith -fno-rtti \
		-ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -lstdc++ -nostartfiles -mabi=ilp32e -march=rv32ec \
		-specs=nano.specs -specs=nosys.specs \
		-fno-exceptions -fno-use-cxa-atexit \
		-Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
#
	
all:	$(BLD)main.bin $(BLD)main.lst $(BLD)main.elf

#-------------------------------------- HEX -----------------------------------
$(BLD)main.bin: $(BLD)main.elf
	$(OBJC) $(BLD)main.elf $(BLD)main.bin -O binary
	$(OBJC) $(BLD)main.elf $(BLD)main.hex -O ihex 
$(BLD)main.lst: $(BLD)main.elf
	$(OBJD) -D $(BLD)main.elf > $(BLD)main.lst

#------------------------------------  LINKER ---------------------------------
oSRC = $(patsubst $(SRC)%.cpp,$(BLD)%.o,$(wildcard $(SRC)*.cpp))
oLIB = $(patsubst $(LIB)%.cpp,$(BLD)%.o,$(wildcard $(LIB)*.cpp))
oPER = $(patsubst $(perSRC)%.c,$(BLD)%.o,$(wildcard $(perSRC)*.c))
$(BLD)main.elf: $(oSRC)  $(BLD)core_riscv.o $(BLD)startup.o
	$(CC) -o $@ -T$(LIB)Link.ld $^ -I$(LIB) $(LFLAGS)
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
includes = $(wildcard $(INC)/*.h)
$(BLD)%.o: $(LIB)%.cpp $(includes)
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(LIB) $(CPPFLAGS)	
# ASM startup
#$(BLD)startup_ch32v30x_D8.o: $(LIB)startup_ch32v30x_D8.S
#	$(CC) -c $(LIB)startup_ch32v30x_D8.S -o $(BLD)startup_ch32v30x_D8.o 	
#
#----------------------- Peripheral C -----------------------------------------
$(BLD)%.o: $(perSRC)%.c
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD) $(lwipBLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) -I$(perINC) $(CFLAGS)
#---------------------- Basic functions ---------------------------------------	
$(BLD)%.o: $(SRC)%.cpp $(includes)
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CPP) $< -o $@ -I$(INC) -I$(perINC) -I$(LIB) $(CPPFLAGS)
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
