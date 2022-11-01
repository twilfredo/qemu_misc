cmake_minimum_required(VERSION 3.16)
include("arm.cmake")

project(main C)
set(MY_EXEC "program")

#corrosion
include(FetchContent)
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.2.2
)
set(Rust_CARGO_TARGET "armv5te-unknown-linux-gnueabi")
FetchContent_MakeAvailable(Corrosion)

#include dirs
include_directories (${CMAKE_BINARY_DIR}/cargo/build/armv5te-unknown-linux-gnueabi/cxxbridge/rust_side/src/
                     ${CMAKE_BINARY_DIR}/cargo/build/armv5te-unknown-linux-gnueabi/cxxbridge/rust/
                     ${CMAKE_CURRENT_SOURCE_DIR}/
                     ${CMAKE_CURRENT_SOURCE_DIR}/rust_side/src/
                     )

#general
add_executable(${MY_EXEC} main.cpp start.s)
set_target_properties(${MY_EXEC} PROPERTIES OUTPUT_NAME "app.elf")
set(ASMFILES start.s)
set(SRCLIST main.cpp)

#compiler
target_compile_options(${MY_EXEC} PRIVATE  
    "-mcpu=arm926ej-s"
     )

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s -g -nostartfiles -nostdlib -g -Wall -fno-exceptions")

#linker
set(LINKER_SCRIPT "linker.ld")
set_target_properties(${MY_EXEC} PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT})
target_link_options(${MY_EXEC} PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

#rust stuff
corrosion_import_crate(MANIFEST_PATH rust_side/Cargo.toml
                       NO_STD
                       PROFILE release
                      )
corrosion_add_target_rustflags(rust_side --cfg cxx_experimental_no_alloc)                   
target_link_libraries(${MY_EXEC}
  PUBLIC
  -nostartfiles
  -nostdlib
  rust_side
)

#objcopy elf->bin
set(CMAKE_OBJCOPY "arm-none-eabi-objcopy")
add_custom_command(TARGET ${MY_EXEC} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
    -O binary app.elf app.bin COMMENT "Converting ELF to binary")

#runner
add_custom_target(run)
add_custom_command(TARGET run POST_BUILD COMMAND 
                  qemu-system-arm -M versatilepb -m 128M -nographic -kernel app.bin
                  COMMENT "Running QEMU...")