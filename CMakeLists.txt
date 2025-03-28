set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.20)
project(Flood VERSION 1.0)

# The application's name.
set(AppName flood)

option(
    FENNTON_FLOOD_ADD_SUBDIR
    "Whether the output binaries should be inside subdirectores based on the build type.
    Also applies to the \"build\" directory."
    OFF
)

if (FENNTON_FLOOD_ADD_SUBDIR)
    # Gets the output directory's name based on the build type.
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set(OutputSubdir "/debug")
    elseif(CMAKE_BUILD_TYPE STREQUAL Release)
        set(OutputSubdir "/release")
    else()
        set(OutputSubdir "/${CMAKE_BUILD_TYPE}")
    endif()
else()
    set(OutputSubdir "")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

# Sets the output directories.

set_target_properties(${AppName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}"
)
set_target_properties(${AppName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}"
)
set_target_properties(${AppName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}"
)

# Adds the executable which will be linked to the core program code.
add_executable(main "${PROJECT_SOURCE_DIR}/src/EntryPoint.cpp")

# Sets the output directories.

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/bin${OutputSubdir}"
)
set_target_properties(main PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}"
)
set_target_properties(main PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}/lib${OutputSubdir}"
)
# Sets the executable's name.
set_target_properties(main PROPERTIES OUTPUT_NAME flood)

target_compile_definitions(main PUBLIC
)

target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/include")
# Link with the library containing the core code.
target_link_libraries(main PUBLIC ${AppName})