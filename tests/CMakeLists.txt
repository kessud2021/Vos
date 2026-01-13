# Tests CMakeLists.txt

# Unit tests
add_executable(test-kernel kernel_test.c)
target_link_libraries(test-kernel PRIVATE kernel-core libc)
add_test(NAME KernelTests COMMAND test-kernel)

add_executable(test-memory memory_test.c)
target_link_libraries(test-memory PRIVATE kernel-core libc)
add_test(NAME MemoryTests COMMAND test-memory)

add_executable(test-scheduler scheduler_test.c)
target_link_libraries(test-scheduler PRIVATE kernel-core libc)
add_test(NAME SchedulerTests COMMAND test-scheduler)

add_executable(test-shell shell_test.c)
target_link_libraries(test-shell PRIVATE libc)
add_test(NAME ShellTests COMMAND test-shell)

# Integration tests
add_executable(test-integration integration_test.c)
target_link_libraries(test-integration PRIVATE kernel-core libc)
add_test(NAME IntegrationTests COMMAND test-integration)

# Enable coverage
if(ENABLE_COVERAGE)
    add_definitions(-fprofile-arcs -ftest-coverage)
    target_link_options(test-kernel PRIVATE --coverage)
endif()
