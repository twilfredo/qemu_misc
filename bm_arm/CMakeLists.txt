cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)


project (bm_arm C ASM)


set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "arm-none-eabi-as")
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")

file(GLOB LINKSCRIPT "linker.ld")
set(ASMFILES start.s)
set(SRCLIST main.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s -g -nostartfiles -nostdlib -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKSCRIPT}")
set(ASMFLAGS "-mcpu=arm926ej-s -g")

add_executable(bm_arm ${SRCLIST} ${ASMFILES})
set_target_properties(bm_arm PROPERTIES OUTPUT_NAME "bm_arm.elf")


add_custom_command(TARGET bm_arm POST_BUILD COMMAND ${CMAKE_OBJCOPY}
    -O binary bm_arm.elf bm_arm.bin COMMENT "Converting ELF to binary")

add_custom_target(run)
add_custom_command(TARGET run POST_BUILD COMMAND 
                  qemu-system-arm -M versatilepb -m 128M -nographic -kernel bm_arm.bin
                  COMMENT "Running QEMU...")