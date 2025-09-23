set(ProgramName fennton_grafik)

add_library(glad STATIC
	"${PROJECT_SOURCE_DIR}/depends/glad/src/glad.c"
)
target_include_directories(glad PUBLIC
	"${PROJECT_SOURCE_DIR}/depends/glad/include"
)
add_library(stb_image STATIC
	"${PROJECT_SOURCE_DIR}/depends/stb/stb_image.h"
)
set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(stb_image PUBLIC
	STB_IMAGE_IMPLEMENTATION
)
add_library(${ProgramName} STATIC
	"Window.cpp"
	"Shader.cpp"
	"Time.cpp"
)
target_link_libraries(${ProgramName} PUBLIC glfw glm glad stb_image)
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