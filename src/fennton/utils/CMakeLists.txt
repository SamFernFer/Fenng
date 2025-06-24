set(ProgramName fennton_utils)

add_library(${ProgramName} STATIC
    "Console.cpp"
)
target_include_directories(fennton_utils PUBLIC ${IncludeDir})

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

set_target_properties(${ProgramName} PROPERTIES OUTPUT_NAME ${ProgramName})