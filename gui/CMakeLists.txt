# GUI CMakeLists.txt

add_library(libgui STATIC)

target_sources(libgui PRIVATE
    windowing/window.cpp
    windowing/event.cpp
    toolkit/widget.cpp
    toolkit/button.cpp
    toolkit/textbox.cpp
    toolkit/window.cpp
)

target_include_directories(libgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(libgui PUBLIC libcpp)

# GUI applications
add_executable(gui-terminal apps/terminal.cpp)
target_link_libraries(gui-terminal PRIVATE libgui)

add_executable(gui-filemgr apps/file_manager.cpp)
target_link_libraries(gui-filemgr PRIVATE libgui)

# Install
install(TARGETS gui-terminal gui-filemgr DESTINATION bin)
