cmake_minimum_required(VERSION 2.8.11)
update_compiler_settings()

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB PRIVATE_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB TEST_SCRIPTS         ${CMAKE_CURRENT_LIST_DIR}/test_scripts/*.lua)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/test/ TEST_FILES)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
source_group ("Private headers"     FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"               FILES ${TEST_FILES})
source_group ("Scripts"             FILES ${TEST_SCRIPTS})

# Form binaries
add_library(Overmind STATIC
    ${HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES}
)

add_executable(OvermindTest
    ${TEST_FILES}
    ${TEST_SCRIPTS}
)

set_property (TARGET Overmind      PROPERTY FOLDER "Libraries")
set_property (TARGET OvermindTest  PROPERTY FOLDER "UnitTests")

# set include directories
# set property for library 
set_property(
    TARGET Overmind
    PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${lua_SOURCE_DIR}/src
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/src
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Utils
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Utils/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include
)

#set property for the test executable
set_property(
    TARGET OvermindTest 
    PROPERTY INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${lua_SOURCE_DIR}/src
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/src
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Utils
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Utils/include
    APPEND PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include
)

# add required definitions (tr1 tuple is not supported under VS11)
set_target_properties(OvermindTest         PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)

add_dependencies(Overmind Constructor lua)
target_link_libraries(Overmind Constructor lua)

add_dependencies(OvermindTest Overmind gtest)
target_link_libraries(OvermindTest Overmind gtest)

add_custom_command(TARGET OvermindTest
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_LIST_DIR}/test_scripts/empty.lua             ${CMAKE_CURRENT_BINARY_DIR}/test_scripts/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_LIST_DIR}/test_scripts/registerObject.lua    ${CMAKE_CURRENT_BINARY_DIR}/test_scripts/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_LIST_DIR}/test_scripts/building.lua          ${CMAKE_CURRENT_BINARY_DIR}/test_scripts/
)

# add tests
add_test(
    NAME    OvermindUnitTest
    WORKING_DIRECTORY OvermindTest_BINARY_DIR
    COMMAND OvermindTest
)

