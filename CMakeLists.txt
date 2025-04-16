cmake_minimum_required(VERSION 3.7)
project(config CXX)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_STANDARD 14)
set(PROJECT_TARGET config)

set(INCLUDES_DIR include)

file(GLOB LIB_HEADERS ${INCLUDES_DIR}/*.h)
file(GLOB LIB_SOURCES src/*.cpp)

add_library(${PROJECT_TARGET} SHARED ${LIB_SOURCES} ${LIB_HEADERS})

set_target_properties(${PROJECT_TARGET}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)

target_include_directories(${PROJECT_TARGET} PUBLIC ${INCLUDES_DIR})

if(GLOBAL_NAME_SEC)
target_compile_options(${PROJECT_TARGET} PRIVATE -DGLOBAL_NAME_SEC=\"${GLOBAL_NAME_SEC}\")
message("USE GLOBAL_NAME_SEC = ${GLOBAL_NAME_SEC}")
endif()
if(SEPARATOR_CHAR)
target_compile_options(${PROJECT_TARGET} PRIVATE -DSEPARATOR_CHAR=\'${SEPARATOR_CHAR}\')
message("USE SEPARATOR_CHAR = ${SEPARATOR_CHAR}")
endif()
if(COMMENT_CHAR)
target_compile_options(${PROJECT_TARGET} PRIVATE -DCOMMENT_CHAR=\'${COMMENT_CHAR}\')
message("USE COMMENT_CHAR = ${COMMENT_CHAR}")
endif()
if(START_SECTION_CHAR)
target_compile_options(${PROJECT_TARGET} PRIVATE -DSTART_SECTION_CHAR=\'${START_SECTION_CHAR}\')
message("USE START_SECTION_CHAR = ${START_SECTION_CHAR}")
endif()
if(STOP_SECTION_CHAR)
target_compile_options(${PROJECT_TARGET} PRIVATE -DSTOP_SECTION_CHAR=\'${STOP_SECTION_CHAR}\')
message("USE STOP_SECTION_CHAR = ${STOP_SECTION_CHAR}")
endif()

#=============================================================================
if(DEBUG_MEM)
# Debug (+ debug +DebugNenory information for gdb)
	target_compile_options(${PROJECT_TARGET} PRIVATE -g -Wall -DDebug -DDebugMem)
elseif(DEBUG)
# Debug (+ debug information for gdb)
	target_compile_options(${PROJECT_TARGET} PRIVATE -g -Wall -DDebug)
else()
# NO Debug (prodaction)
	target_compile_options(${PROJECT_TARGET} PRIVATE -g -Wall)
endif()

#=============================================================================
#=============	TEST 
#=============================================================================
set(PROJECT_TARGET_TEST test1)
file(GLOB TEST_HEADERS ${INCLUDES_DIR}/*.h)
file(GLOB TEST_SOURCES test/*.cpp)

add_executable(${PROJECT_TARGET_TEST} ${TEST_HEADERS} ${TEST_SOURCES})
set_target_properties(${PROJECT_TARGET_TEST}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)
if(DEBUG)
# Debug (+ debug information for gdb)
	target_compile_options(${PROJECT_TARGET_TEST} PRIVATE -g -Wall -DDebug)
else()
# NO Debug (prodaction)
	target_compile_options(${PROJECT_TARGET_TEST} PRIVATE -g -Wall)
endif()

target_include_directories(${PROJECT_TARGET_TEST} PUBLIC ${INCLUDES_DIR})
target_link_libraries(${PROJECT_TARGET_TEST} ${PROJECT_TARGET})

