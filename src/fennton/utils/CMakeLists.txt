set(ProgramName fennton_utils)

add_library(${ProgramName} STATIC
	"${PROJECT_SOURCE_DIR}/depends/whereami/src/whereami.c"
	"Console.cpp"
	"Text.cpp"
	"Filesystem.cpp"
)

target_include_directories(${ProgramName} PUBLIC
	${IncludeDir}
	"${PROJECT_SOURCE_DIR}/depends/whereami/src"
)
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