add_executable(console_init "ConsoleInit.cpp")
target_link_libraries(console_init fennton_utils)
target_include_directories(console_init PUBLIC ${IncludeDir})

add_executable(console_print "ConsolePrint.cpp")
target_link_libraries(console_print fennton_utils)
target_include_directories(console_print PUBLIC ${IncludeDir})