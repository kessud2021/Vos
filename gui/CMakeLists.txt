# GUI Library CMakeLists.txt

# Windowing module
set(WINDOWING_SOURCES
    windowing/window.cpp
    windowing/window_manager.cpp
)

set(WINDOWING_HEADERS
    windowing/window.hpp
    windowing/window_manager.hpp
)

# Toolkit module
set(TOOLKIT_SOURCES
    toolkit/widget.cpp
)

set(TOOLKIT_HEADERS
    toolkit/widget.hpp
)

# Applications
set(APPS_SOURCES
    apps/terminal_app.cpp
    apps/file_manager_app.cpp
)

set(APPS_HEADERS
    apps/terminal_app.hpp
    apps/file_manager_app.hpp
)

# Create main GUI library
add_library(libgui STATIC
    ${WINDOWING_SOURCES}
    ${TOOLKIT_SOURCES}
    ${APPS_SOURCES}
)

# Set library properties
set_target_properties(libgui PROPERTIES
    OUTPUT_NAME gui
    LINKER_LANGUAGE CXX
)

# Include directories
target_include_directories(libgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against kernel if needed
target_link_libraries(libgui PRIVATE kernel-core)

# Installation
install(FILES ${WINDOWING_HEADERS} DESTINATION include/gui/windowing)
install(FILES ${TOOLKIT_HEADERS} DESTINATION include/gui/toolkit)
install(FILES ${APPS_HEADERS} DESTINATION include/gui/apps)
install(TARGETS libgui DESTINATION lib)

message(STATUS "GUI: Windowing module configured")
message(STATUS "GUI: Toolkit module configured")
message(STATUS "GUI: Applications configured")
