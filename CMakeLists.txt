cmake_minimum_required(VERSION 3.7)
project(config)

set(PROJECT_TARGET config)

file(GLOB LIST_SOURCES "src/*.c")
include_directories(include)

add_definitions(-g3 -Wall -lm)
add_executable(${PROJECT_TARGET} ${LIST_SOURCES})
#target_link_libraries(${PROJECT_TARGET} m)
set_target_properties(${PROJECT_TARGET}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

