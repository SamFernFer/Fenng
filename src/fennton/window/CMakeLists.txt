set(ProgramName fennton_window)

add_library(${ProgramName} STATIC
	"Window.cpp"
)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})

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