cmake_minimum_required(VERSION 3.15)
project(FenngTests VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")

set(IncludeDir $IncludeDir "${PROJECT_SOURCE_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/src/gui")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/window")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/fennton/utils")