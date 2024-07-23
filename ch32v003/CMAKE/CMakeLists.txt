cmake_minimum_required(VERSION 3.18)
set(PROJECT CLANGD)

project(${PROJECT})

set(EXECUTABLE_NAME ${PROJECT})
#set(CMAKE_SYSTEM_PROCESSOR riscv32)
set(TOOLCHAIN "/opt/ch32_riscv_gcc_12/bin/riscv-none-elf")
set(DEFAULT_SYSROOT "/opt/ch32_riscv_gcc_12/riscv-none-elf")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN}-g++")
set(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN}-objdump)
set(CMAKE_SIZE ${TOOLCHAIN}-size)
#set(OPTIMIZATION 0)
#set(OPTIMIZATION fast)
set(OPTIMIZATION s)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify compiler flags
set(ARCH_FLAGS "-march=rv32ec -mabi=ilp32e")
set(CMAKE_C_FLAGS " -c -Wall -Wextra -Werror -g -O${OPTIMIZATION} \
    ${ARCH_FLAGS}\
	-fno-pic -fno-common \
	-Wno-pointer-arith \
	-ffast-math -fno-math-errno -ffunction-sections -fdata-sections"
)
set(CMAKE_CXX_FLAGS "-c -std=c++17 -Wall -Wextra -Werror -g3 -O${OPTIMIZATION} \
	${ARCH_FLAGS}\
	-fno-pic -fno-common -D\"assert_param(x)=\" \
	-fno-exceptions -fno-rtti -fno-threadsafe-statics \
	-Wno-pointer-arith \
	-ffast-math -fno-math-errno -ffunction-sections -fdata-sections"
)
set(CMAKE_ASM_FLAGS "-Wall ${ARCH_FLAGS} \
            -x assembler-with-cpp" CACHE STRING "Common flags for assembler")
            set(CMAKE_LD_FLAGS "-nostartfiles ${ARCH_FLAGS} \
	-specs=nano.specs -specs=nosys.specs -fno-exceptions \
	-fno-use-cxa-atexit \
    -Xlinker -Map=${PROJECT}.map -Xlinker --gc-sections -Xlinker -EL"
)

include_directories(inc/ lib/ Peripheral/inc/
    /opt/ch32_riscv_gcc_12/riscv-none-elf/include/
    /opt/ch32_riscv_gcc_12/riscv-none-elf/include/c++/12.2.0/
)
file(GLOB_RECURSE SRCS src/*.cpp lib/*.cpp Peripheral/src/*.c)
add_executable(${PROJECT}.elf ${SRCS})

set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> \
-T../lib/Link.ld \
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

#execute_process(
#    COMMAND ${CMAKE_COMMAND} -E create_symlink
#        ${CMAKE_BINARY_DIR}/compile_commands.json
#        ${CMAKE_SOURCE_DIR}/compile_commands.json
#)
#add_custom_command(
#    TARGET ${PROJECT}
#    COMMAND ${CMAKE_OBJDUMP} -D ${PROJECT} > ${PROJECT}.lst
#)
