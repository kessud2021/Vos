# Core libraries CMakeLists.txt

# libc - POSIX C library
add_library(libc STATIC)
target_sources(libc PRIVATE
    libc/stdlib.c
    libc/string.c
    libc/stdio.c
    libc/unistd.c
    libc/fcntl.c
    libc/signal.c
    libc/errno.c
)
target_include_directories(libc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libc/include
)

# libcpp - C++ standard library (stub)
add_library(libcpp STATIC)
target_sources(libcpp PRIVATE
    libcpp/vector.cpp
    libcpp/string.cpp
    libcpp/map.cpp
)
target_include_directories(libcpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libcpp/include
)

# Export for linking
install(TARGETS libc libcpp DESTINATION lib)
