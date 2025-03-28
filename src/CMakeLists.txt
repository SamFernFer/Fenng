set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

# The application's name.
set(AppName flood)

# Reads the automatically-generated file list.
# file(READ "${PROJECT_SOURCE_DIR}/src/FileList.txt" FileList)

# message(FATAL_ERROR ${FileList})

# Adds the core program's code as a static library.
add_library(${AppName} STATIC
	"${PROJECT_SOURCE_DIR}/depends/glad/src/glad.c"
	"${PROJECT_SOURCE_DIR}/depends/whereami/src/whereami.c"
	"utils/FileUtils.cpp"
	"utils/JSON.cpp"
	"utils/Misc.cpp"
	"ImageLoad.c"
	"Application.cpp"
	"EntryPoint.cpp"
	"Time.cpp"
	"Resources.cpp"
	"Settings.cpp"
	"Window.cpp"
	"Shader.cpp"
	"Texture.cpp"
	"Texture.move.cpp"
)

# GLFW.

# Prevents target name collisions.
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory(
	"${PROJECT_SOURCE_DIR}/depends/glfw"
	"${PROJECT_SOURCE_DIR}/depends/glfw/build"
	EXCLUDE_FROM_ALL
)
# glm.

# Makes sure GLM uses the left-handed coordinate system.
add_compile_definitions(GLM_FORCE_LEFT_HANDED)

add_subdirectory(
	"${PROJECT_SOURCE_DIR}/depends/glm"
	"${PROJECT_SOURCE_DIR}/depends/glm/build"
	EXCLUDE_FROM_ALL
)

# Compilation options.

target_compile_definitions(${AppName} PUBLIC
)
if(MSVC)
	target_compile_options(${AppName} PUBLIC /Zc:preprocessor)
else()
	target_compile_options(${AppName} PUBLIC
		-Wpedantic
		-Werror=return-type
		-Werror=switch
		-Werror=extern-initializer
		-Werror=microsoft-template
	)
endif()

target_include_directories(${AppName} SYSTEM PUBLIC
	"${BOOST_ROOT}"
	"${PROJECT_SOURCE_DIR}/depends/glad/include"
# 	"${PROJECT_SOURCE_DIR}/depends/Nuklear"
)
target_include_directories(${AppName} PUBLIC
    "${PROJECT_SOURCE_DIR}/depends/whereami/src"
	"${PROJECT_SOURCE_DIR}/depends/glfw/include"
	"${PROJECT_SOURCE_DIR}/depends/stb"
	"${PROJECT_SOURCE_DIR}/depends/glm"
	"${PROJECT_SOURCE_DIR}/include"
)
target_link_directories(${AppName} PUBLIC
	"${BOOST_ROOT}/stage/lib"
)
target_link_libraries(${AppName} PUBLIC
	glfw glm
)