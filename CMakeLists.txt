cmake_minimum_required(VERSION 3.7)
project(config)

set(PROJECT_TARGET config)
set(INCLUDES_DIR include)

file(GLOB LIST_SOURCES "src/*.c")
file(GLOB LIST_HEADERS "${INCLUDES_DIR}/*.h")
set(ALL_SOURCES "${LIST_HEADERS};${LIST_SOURCES}")

add_executable(${PROJECT_TARGET} ${ALL_SOURCES})
#add_library(${PROJECT_TARGET} ${ALL_SOURCES})
target_include_directories(${PROJECT_TARGET} PRIVATE ${INCLUDES_DIR})

message("INCLUDE = " ${INCLUDE_DIRECTORIES})

if(GLOBAL_NAME_SEC)
	target_compile_options(${PROJECT_TARGET} PRIVATE -DGLOBAL_NAME_SEC=\"${GLOBAL_NAME_SEC}\")
endif()

if(SEPARATOR_CHAR)
	target_compile_options(${PROJECT_TARGET} PRIVATE -DSEPARATOR_CHAR=\"${SEPARATOR_CHAR}\")
endif()

if(COMMENT_CHAR)
	target_compile_options(${PROJECT_TARGET} PRIVATE -DCOMMENT_CHAR=\"${COMMENT_CHAR}\")
endif()

if(START_SECTION_CHAR)
	target_compile_options(${PROJECT_TARGET} PRIVATE -DSTART_SECTION_CHAR=\"${START_SECTION_CHAR}\")
endif()

if(STOP_SECTION_CHAR)
	target_compile_options(${PROJECT_TARGET} PRIVATE -DSTOP_SECTION_CHAR=\"${STOP_SECTION_CHAR}\")
endif()

target_compile_options(${PROJECT_TARGET} PRIVATE -g -Wall -DDebug)
#target_compile_options(${PROJECT_TARGET} PRIVATE -Wall)

set_target_properties(${PROJECT_TARGET}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

