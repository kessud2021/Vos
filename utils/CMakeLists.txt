# System utilities CMakeLists.txt

add_subdirectory(fileutils)
add_subdirectory(textutils)
add_subdirectory(sysutils)

# File utilities
add_executable(ls fileutils/ls.c)
target_link_libraries(ls PRIVATE libc)

add_executable(cp fileutils/cp.c)
target_link_libraries(cp PRIVATE libc)

add_executable(mv fileutils/mv.c)
target_link_libraries(mv PRIVATE libc)

add_executable(rm fileutils/rm.c)
target_link_libraries(rm PRIVATE libc)

add_executable(mkdir fileutils/mkdir.c)
target_link_libraries(mkdir PRIVATE libc)

# Text utilities
add_executable(cat textutils/cat.c)
target_link_libraries(cat PRIVATE libc)

add_executable(grep textutils/grep.c)
target_link_libraries(grep PRIVATE libc)

add_executable(sed textutils/sed.c)
target_link_libraries(sed PRIVATE libc)

# Install
install(TARGETS ls cp mv rm mkdir cat grep sed DESTINATION bin)
