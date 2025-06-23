cmake_minimum_required(VERSION 3.15)
project(FenngTests VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}")

# Gets the parent directory, so that the main program's headers can be accessed.
cmake_path(GET PROJECT_SOURCE_DIR PARENT_PATH ParentPath)
# Saves the include directory.
set (IncludeDir "${ParentPath}/include")

add_subdirectory("${ParentPath}/src/fennton/utils")

add_subdirectory("${PROJECT_SOURCE_DIR}/src/gui")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/window")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/fennton/utils")