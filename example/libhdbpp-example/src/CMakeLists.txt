cmake_minimum_required(VERSION 3.11)

# source files
set(LOCAL_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/HdbppExampleDbApi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HdbppExampleDb.cpp
    )

set(SRC_FILES 
    ${SRC_FILES} 
    ${LOCAL_SRC_FILES} 
    PARENT_SCOPE)
