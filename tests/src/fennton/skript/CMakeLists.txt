add_executable(skript_tokeniser "Tokeniser.cpp")
target_link_libraries(skript_tokeniser fennton_skript fennton_utils)
target_include_directories(skript_tokeniser PUBLIC ${IncludeDir})