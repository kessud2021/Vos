cmake_minimum_required(VERSION 3.20)
project(VSS-CO-OS C CXX ASM)

# Global settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")

# Architecture detection
if(NOT ARCH)
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
        set(ARCH "x86_64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
        set(ARCH "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "riscv64")
        set(ARCH "riscv64")
    else()
        set(ARCH "x86_64")  # Default
    endif()
endif()

message(STATUS "Building for architecture: ${ARCH}")

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -O2 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2 -march=native")

# Kernel-specific flags
set(KERNEL_C_FLAGS "-fno-asynchronous-unwind-tables -fno-unwind-tables -fno-stack-protector -fno-builtin")
set(KERNEL_LINK_FLAGS "-nostdlib -Wl,--build-id=none")

# Options
option(ENABLE_DEBUG "Enable debug symbols" ON)
option(ENABLE_TESTING "Enable tests" ON)
option(CROSS_COMPILE "Cross-compile for target" OFF)

# Debug symbols
if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
endif()

# Add subdirectories
add_subdirectory(kernel)
add_subdirectory(shell)
add_subdirectory(utils)
add_subdirectory(lib)
add_subdirectory(gui)
add_subdirectory(pkgmgr)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Summary
message(STATUS "Build configuration:")
message(STATUS "  Architecture: ${ARCH}")
message(STATUS "  Debug symbols: ${ENABLE_DEBUG}")
message(STATUS "  Testing: ${ENABLE_TESTING}")
message(STATUS "  Cross-compile: ${CROSS_COMPILE}")
