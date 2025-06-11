set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(ProgramName main)

add_executable(${ProgramName}
    "Main.cpp"
)

target_compile_definitions(${ProgramName} PUBLIC
)
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

set_target_properties(${ProgramName} PROPERTIES OUTPUT_NAME fenng)