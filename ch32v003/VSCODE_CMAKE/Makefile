.PHONY: all clean load erase reset load_gdb

all:
	mkdir -p build/; cd build; cmake ..; make -j99; cd ..;

clean:
	rm -rf build 
	
load:	
	/opt/openocd_wch/bin/openocd -s /usr/local/share/openocd/scripts \
	-f lib/wch-riscv_new.cfg -c init -c halt \
	-c init -c halt -c "program build/main.bin verify exit"
