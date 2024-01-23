cmake_minimum_required(VERSION 3.7)
project(config C)
set(PROJECT_TARGET config)

set(INCLUDES_DIR include)

file(GLOB LIB_HEADERS ${INCLUDES_DIR}/*.h)
file(GLOB LIB_SOURCES src/*.c)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

message(INCLUDE = ${LIB_HEADERS})
message(SOURCE = ${LIB_SOURCES})
add_library(${PROJECT_TARGET} ${LIB_SOURCES} ${LIB_HEADERS})
target_include_directories(${PROJECT_TARGET} PUBLIC ${INCLUDES_DIR})

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

set(PROJECT_TARGET_TEST test1)
file(GLOB TEST_HEADERS ${INCLUDES_DIR}/*.h)
file(GLOB TEST_SOURCES test/*.c)
add_executable(${PROJECT_TARGET_TEST} ${TEST_HEADERS} ${TEST_SOURCES})
target_include_directories(${PROJECT_TARGET_TEST} PUBLIC ${INCLUDES_DIR})
target_link_libraries(${PROJECT_TARGET_TEST} ${PROJECT_TARGET})

