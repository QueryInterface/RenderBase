cmake_minimum_required(VERSION 2.8.11)

# Include directories
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/Utils/include)

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB PRIVATE_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  SOURCE_FILES)

# Form binaries
add_library(ResourceOverseer SHARED
    ${HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(ResourceOverseer)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
source_group ("Private headers"     FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"               FILES ${TEST_FILES})

set_property (TARGET ResourceOverseer      PROPERTY FOLDER "Libraries")

add_definitions(-DDEFINE_EXPORT)
