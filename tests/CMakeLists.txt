cmake_minimum_required(VERSION 3.15)
project(FenngTests VERSION 1.0)

# Gets the output directory's prefix based on the build type.
if (
    CMAKE_BUILD_TYPE STREQUAL Debug
    OR CMAKE_BUILD_TYPE STREQUAL debug
)
    set(OutputSubdir "/debug")
elseif(
    CMAKE_BUILD_TYPE STREQUAL Release
    OR CMAKE_BUILD_TYPE STREQUAL release
)
    set(OutputSubdir "/release")
else()
    set(OutputSubdir "/${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")

add_subdirectory("${PROJECT_SOURCE_DIR}/src/gui")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/window")