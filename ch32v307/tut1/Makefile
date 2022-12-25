# Compiler:
# arm-none-eabi-gcc --version: === 8.3.1 20190703
# System debugger:
# gdb-multiarch --version:	===	GNU gdb 10.1.90.20210103
# Open On-Chip Debugger:	
# openocd --version === Open On-Chip Debugger 0.11.0 (2021-10-07-10:32)
.PHONY: all clean load erase
#_________________________ GCC compilator commands ____________________________
CC = arm-none-eabi-g++
OBJC = arm-none-eabi-objcopy
OBJD = arm-none-eabi-objdump
AS = arm-none-eabi-as
LD = arm-none-eabi-ld

INC = inc/
LIB = lib/
BLD = build/
SRC = src/
#///////////////////////////////////
CPPFLAGS = -c -std=c++17 -Wall -Wextra -g -O0 \
	   -fno-pic -fno-common -D"assert_param(x)=" \
	   -mcpu=cortex-m4 -mthumb -fno-exceptions \
	   -Wno-pointer-arith -fno-rtti -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	   -ffast-math -fno-math-errno -ffunction-sections -fdata-sections
LFLAGS = -mthumb -march=armv7e-m -nostartfiles \
		 -specs=nano.specs -specs=nosys.specs -fno-exceptions \
		 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
		 -fno-use-cxa-atexit \
		 -Xlinker -Map=$(BLD)main.map -Xlinker --gc-sections
#-u_printf_float 
#для FPU
	
all:	$(BLD)main.bin $(BLD)main.lst $(BLD)main.elf
$(BLD)main.bin: $(BLD)main.elf
	arm-none-eabi-objcopy $(BLD)main.elf $(BLD)main.bin -O binary
	arm-none-eabi-objcopy $(BLD)main.elf $(BLD)main.hex -O ihex 
$(BLD)main.lst: $(BLD)main.elf
	arm-none-eabi-objdump -D $(BLD)main.elf > $(BLD)main.lst

#------------------------------------  LINKER ---------------------------------
$(BLD)main.elf: $(BLD)startup.o $(BLD)system_stm32f4xx.o $(BLD)rcc.o
$(BLD)main.elf: $(BLD)main.o				
	$(CC) -o $@ -T$(LIB)stm32f4.ld $^ -I$(LIB) $(LFLAGS)
#    $@ — target name
#    $< — first dependancy name
#    $? — all dependancies names, thats newer than target
#    $^ — all dependencies names of the target
#	 %	- template for all files

#-------------------------------------- HEX -----------------------------------
	arm-none-eabi-objcopy -j .text -j .data -j .bss -O ihex \
	$(BLD)main.elf $(BLD)main.hex
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
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) $(CPPFLAGS)	
#_____________________________________________________________________________
#_________________________ Basic functions ___________________________________	
#_____________________________________________________________________________
$(BLD)%.o: $(SRC)%.cpp
	@ # **** mutex for parallel execution (mkdir must be locked) ****
	@if mkdir /var/lock/.myscript.exclusivelock; \
	then if [ ! -d "$(BLD)" ]; then mkdir $(BLD); fi; \
	rmdir /var/lock/.myscript.exclusivelock; \
	fi
	$(CC) $< -o $@ -I$(INC) -I$(LIB) $(CPPFLAGS)
clean:
	rm -rf build 
	
load:	$(BLD)main.bin
	openocd -f lib/stlink.cfg -f lib/stm32f4x.cfg -c "program $(BLD)main.bin \
	verify reset exit 0x08000000"

erase:
	openocd -f lib/stlink.cfg -f lib/stm32f4x.cfg -c "init" -c \
		"halt" -c "wait_halt" -c "stm32f4x mass_erase 0" -c "exit"
