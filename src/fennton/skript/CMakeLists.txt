set(ProgramName fennton_skript)

add_library(${ProgramName} STATIC
	"Parser.cpp"
)
target_link_libraries(${ProgramName} PUBLIC)
target_include_directories(${ProgramName} PUBLIC ${IncludeDir})
target_include_directories(${ProgramName} SYSTEM PUBLIC ${SystemIncludeDir})

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