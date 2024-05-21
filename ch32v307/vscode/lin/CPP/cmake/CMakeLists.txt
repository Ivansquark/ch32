cmake_minimum_required(VERSION 3.18)
set(PROJECT mamo_mrt_cmake)

project(${PROJECT})

set(EXECUTABLE_NAME ${PROJECT})
set(TOOLCHAIN /opt/ch32_riscv_gcc/bin/riscv-none-elf)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN}-objdump)
set(CMAKE_SIZE ${TOOLCHAIN}-size)
set(OPTIMIZATION 0)
set(CMAKE_C_FLAGS " -c -Wall -Wextra -Werror -g -O${OPTIMIZATION} \
    -march=rv32imafc -mabi=ilp32f \
	-fno-pic -fno-common \
	-Wno-pointer-arith \
	-ffast-math -fno-math-errno -ffunction-sections -fdata-sections"
)
set(CMAKE_CXX_FLAGS "-c -std=c++17 -Wall -Wextra -Werror -g3 -O${OPTIMIZATION} \
	-march=rv32imafc -mabi=ilp32f \
	-fno-pic -fno-common -D\"assert_param(x)=\" \
	-fno-exceptions -fno-rtti -fno-threadsafe-statics \
	-Wno-pointer-arith \
	-ffast-math -fno-math-errno -ffunction-sections -fdata-sections"
)
set(CMAKE_LD_FLAGS "-nostartfiles -march=rv32imafc -mabi=ilp32f \
	-specs=nano.specs -specs=nosys.specs -fno-exceptions \
	-fno-use-cxa-atexit \
    -Xlinker -Map=${PROJECT}.map -Xlinker --gc-sections -Xlinker -EL"
)

include_directories(inc/ lib/ Peripheral/inc/
    LWIP/ LWIP/include/ LWIP/system/
    /opt/ch32_riscv_gcc/riscv-none-elf/include/
)
file(GLOB_RECURSE SRCS src/*.cpp lib/*.cpp
    LWIP/core/*.c LWIP/core/ipv4/*.c LWIP/netif/*.c LWIP/*.cpp 
    LWIP/net_led_tmr.c
    LWIP/system/*.c
    Peripheral/src/*.c
)
add_executable(${PROJECT}.elf ${SRCS})

set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> \
-T../../lib/Link.ld \
-o ${PROJECT_NAME}.elf  <OBJECTS>  ${CMAKE_LD_FLAGS} \
 -Ilib/ "
)


add_custom_command(
    TARGET ${PROJECT}.elf
    POST_BUILD
    COMMAND  echo -------------------------------
    COMMAND  echo --------- POSTBUILD -----------
    COMMAND  echo -------------------------------
    COMMAND ${CMAKE_SIZE} ${PROJECT}.elf
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT}.elf ${PROJECT}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT}.elf ${PROJECT}.hex
    COMMAND ${CMAKE_OBJDUMP} -D ${PROJECT}.elf > ${PROJECT}.lst
)
#add_custom_command(
#    TARGET ${PROJECT}
#    COMMAND ${CMAKE_OBJDUMP} -D ${PROJECT} > ${PROJECT}.lst
#)
