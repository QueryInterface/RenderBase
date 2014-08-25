cmake_minimum_required(VERSION 2.8.11)
project(lua CXX C)

update_compiler_settings()

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  SOURCE_FILES)

# Form binaries
add_library(lua STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
