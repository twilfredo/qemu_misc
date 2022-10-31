cmake_minimum_required(VERSION 3.16)
include("arm.cmake")

project(main C)
set(MY_EXEC "program")

add_executable(${MY_EXEC} main.cpp start.s)
set_target_properties(${MY_EXEC} PROPERTIES OUTPUT_NAME "app.elf")
set(ASMFILES start.s)
set(SRCLIST main.cpp)

#compiler
target_compile_options(${MY_EXEC} PRIVATE  
    "-mcpu=arm926ej-s"
     )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s -g -nostartfiles -nostdlib -g -Wall")

#linker
set(LINKER_SCRIPT "linker.ld")
set_target_properties(${MY_EXEC} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT})
target_link_options(${MY_EXEC} PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

#objcopy elf->bin
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")
add_custom_command(TARGET ${MY_EXEC} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
    -O binary app.elf app.bin COMMENT "Converting ELF to binary")

#runner
add_custom_target(run)
add_custom_command(TARGET run POST_BUILD COMMAND 
                  qemu-system-arm -M versatilepb -m 128M -nographic -kernel app.bin
                  COMMENT "Running QEMU...")