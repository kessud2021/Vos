# Kernel CMakeLists.txt
add_library(kernel-core STATIC)

# Architecture-specific
if(ARCH STREQUAL "x86_64")
    target_sources(kernel-core PRIVATE
        arch/x86_64/boot.s
        arch/x86_64/cpu.c
        arch/x86_64/mmu.c
        arch/x86_64/interrupts.s
        arch/x86_64/context.c
    )
    target_include_directories(kernel-core PRIVATE arch/x86_64)
endif()

# Core kernel
target_sources(kernel-core PRIVATE
    core/main.c
    core/scheduler.c
    core/memory.c
    core/ipc.c
    core/sync.c
    fs/vfs.c
    fs/ext4.c
    drivers/console.c
    drivers/block.c
    mm/page.c
    mm/vma.c
    lib/string.c
    lib/assert.c
)

# Include paths
target_include_directories(kernel-core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/include
)

# Kernel-specific flags
target_compile_options(kernel-core PRIVATE
    ${KERNEL_C_FLAGS}
    -fno-omit-frame-pointer
)

# Kernel ELF
add_executable(kernel-elf)
target_sources(kernel-elf PRIVATE kernel-entry.c)
target_link_libraries(kernel-elf PRIVATE kernel-core)
target_link_options(kernel-elf PRIVATE ${KERNEL_LINK_FLAGS})

# Linker script
target_link_options(kernel-elf PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

# Create bootable image
add_custom_command(TARGET kernel-elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:kernel-elf> ${CMAKE_BINARY_DIR}/kernel.bin
    COMMENT "Creating kernel binary"
)
