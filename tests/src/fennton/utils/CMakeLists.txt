add_executable(console_init "ConsoleInit.cpp")
target_link_libraries(console_init fennton_utils)
target_include_directories(console_init PUBLIC ${IncludeDir})