# Test: Window Manip
set(ProgramName window_manip)

add_executable(${ProgramName} "Manipulation.cpp")
target_link_libraries(${ProgramName} fennton_utils fennton_grafik)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})
target_include_directories(${ProgramName} SYSTEM PUBLIC ${SystemIncludeDir})

if(MSVC)
	target_compile_options(${ProgramName} PUBLIC /Zc:preprocessor)
else()
	target_compile_options(${ProgramName} PUBLIC
		-Wpedantic
		-Werror=return-type
		# -Wno-switch
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

# Test: Grafik Triangle
set(ProgramName grafik_tri)

add_executable(${ProgramName} "Triangle.cpp")
target_link_libraries(${ProgramName} fennton_utils fennton_grafik)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})
target_include_directories(${ProgramName} SYSTEM PUBLIC ${SystemIncludeDir})

if(MSVC)
	target_compile_options(${ProgramName} PUBLIC /Zc:preprocessor)
else()
	target_compile_options(${ProgramName} PUBLIC
		-Wpedantic
		-Werror=return-type
		# -Wno-switch
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()


# Test: Grafik Rectangle
set(ProgramName grafik_rect)

add_executable(${ProgramName} "Rectangle.cpp")
target_link_libraries(${ProgramName} fennton_utils fennton_grafik)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})
target_include_directories(${ProgramName} SYSTEM PUBLIC ${SystemIncludeDir})

if(MSVC)
	target_compile_options(${ProgramName} PUBLIC /Zc:preprocessor)
else()
	target_compile_options(${ProgramName} PUBLIC
		-Wpedantic
		-Werror=return-type
		# -Wno-switch
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

# Test: Grafik Uniform
set(ProgramName grafik_uniform)

add_executable(${ProgramName} "Uniform.cpp")
target_link_libraries(${ProgramName} fennton_utils fennton_grafik)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})
target_include_directories(${ProgramName} SYSTEM PUBLIC ${SystemIncludeDir})

if(MSVC)
	target_compile_options(${ProgramName} PUBLIC /Zc:preprocessor)
else()
	target_compile_options(${ProgramName} PUBLIC
		-Wpedantic
		-Werror=return-type
		# -Wno-switch
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()