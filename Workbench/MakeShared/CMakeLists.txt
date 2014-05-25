cmake_minimum_required (VERSION 2.8.11)

set (3RDPARTY_DIR ${PROJECT_SOURCE_DIR}/../3rdparty)

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/  SOURCE_FILES)

# Form binaries
add_executable(MakeShared
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
 