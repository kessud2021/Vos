# Shell CMakeLists.txt
add_executable(vss-shell)

target_sources(vss-shell PRIVATE
    main.c
    lexer.c
    parser.c
    executor.c
    builtins.c
    env.c
    jobs.c
    history.c
    completion.c
    lineread.c
)

target_include_directories(vss-shell PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(vss-shell PRIVATE
    -Wall -Wextra -O2
)

# Install
install(TARGETS vss-shell DESTINATION bin)
