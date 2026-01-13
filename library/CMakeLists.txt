# VOS Integration Library CMakeLists.txt

set(VOS_LIBRARY_SOURCES
    Vos.c
)

set(VOS_LIBRARY_HEADERS
    Vos.h
)

# Create static library
add_library(vos STATIC ${VOS_LIBRARY_SOURCES})

# Set properties
set_target_properties(vos PROPERTIES
    OUTPUT_NAME vos
    LINKER_LANGUAGE C
)

# Include directories
target_include_directories(vos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against kernel
target_link_libraries(vos PRIVATE kernel-core)

# Installation
install(FILES ${VOS_LIBRARY_HEADERS} DESTINATION include/vos)
install(TARGETS vos DESTINATION lib)

message(STATUS "VOS Library: Built as static library 'libvos.a'")
