add_executable(window_manip "Manipulation.cpp")
target_link_libraries(window_manip fennton_utils fennton_window)
target_include_directories(window_manip PUBLIC ${IncludeDir})