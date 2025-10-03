cmake_minimum_required(VERSION 3.15)
project(Fenng VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

if (FENNTON_OUTPUT_PREFIX)
    set(OutputSubdir "/${FENNTON_OUTPUT_PREFIX}")
else()
    # Empties the prefix.
    set(OutputSubdir "")
endif()

set(IncludeDir
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/depends/glfw/include"
    "${PROJECT_SOURCE_DIR}/depends/glm"
    "${PROJECT_SOURCE_DIR}/depends/stb"
)
set(SystemIncludeDir
    "${PROJECT_SOURCE_DIR}/depends/glad/include"
)

option(FENNTON_BUILD_TESTS "Whether to build the tests." OFF)
option(FENNTON_BUILD_MAIN "Whether to build the main program." ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")

# Adds the external libraries.
add_subdirectory("${PROJECT_SOURCE_DIR}/depends/glfw")
add_subdirectory("${PROJECT_SOURCE_DIR}/depends/glm")
target_compile_definitions(
    glm PUBLIC "GLM_FORCE_LEFT_HANDED"
)

# Adds the individual core libraries.
add_subdirectory("${PROJECT_SOURCE_DIR}/src/fennton/utils")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/fennton/grafik")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/fennton/skript")

# Optionally builds the main executable.
if (FENNTON_BUILD_MAIN)
    add_subdirectory("${PROJECT_SOURCE_DIR}/src/main")
endif()

# Optionally builds the tests (manual and automatic).
if (FENNTON_BUILD_TESTS)
    set(FullDir "tests/build${OutputSubdir}")
    file(MAKE_DIRECTORY ${FullDir})
    add_subdirectory("tests" ${FullDir})
endif()