set(ProgramName fennton_gl)

add_library(${ProgramName} STATIC
	"${PROJECT_SOURCE_DIR}/depends/glad/src/glad.c"
	"Window.cpp"
)
target_link_libraries(${ProgramName} PUBLIC glfw glm)
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