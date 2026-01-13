cmake_minimum_required(VERSION 3.20)
project(VSS-CO-OS C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global settings
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Architecture detection
if(NOT ARCH)
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
        set(ARCH "x86_64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
        set(ARCH "arm64")
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "riscv64")
        set(ARCH "riscv64")
    else()
        set(ARCH "x86_64")  # Default
    endif()
endif()

message(STATUS "================================")
message(STATUS "  VSS-CO OS Build Configuration")
message(STATUS "================================")
message(STATUS "Architecture: ${ARCH}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Kernel-specific flags (split to avoid issues)
set(KERNEL_C_FLAGS "-fno-stack-protector -fno-builtin")
set(KERNEL_LINK_FLAGS "")

# Options
option(ENABLE_DEBUG "Enable debug symbols" ON)
option(ENABLE_TESTING "Enable tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Debug symbols
if(ENABLE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -DDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -DDEBUG")
    message(STATUS "Debug mode: ENABLED")
else()
    message(STATUS "Debug mode: DISABLED")
endif()

# Coverage
if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    message(STATUS "Code coverage: ENABLED")
endif()

# Build single OS image (not separate binaries)
add_subdirectory(kernel)
# add_subdirectory(shell)       # TODO: Integrate shell into kernel
# add_subdirectory(utils)
# add_subdirectory(lib)
# add_subdirectory(gui)
# add_subdirectory(pkgmgr)

# Test harness (runs kernel on Windows without virtualization)
add_executable(test-kernel test-kernel.c)
target_link_libraries(test-kernel PRIVATE kernel-core)
target_include_directories(test-kernel PRIVATE kernel/include)

if(ENABLE_TESTING)
    enable_testing()
    # add_subdirectory(tests)  # TODO: Fix libc dependencies
    message(STATUS "Testing: ENABLED (pending libc)")
else()
    message(STATUS "Testing: DISABLED")
endif()

message(STATUS "================================")
message(STATUS "To build: cd build && ninja")
message(STATUS "To test: ninja test")
message(STATUS "To clean: rm -rf build/")
message(STATUS "================================")
