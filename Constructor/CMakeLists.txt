cmake_minimum_required(VERSION 2.8.11)

project(Constructor CXX)

update_compiler_settings()

# Define source groups

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB PRIVATE_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  SOURCE_FILES)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/test/ TEST_FILES)


# Form binaries
add_library(Constructor STATIC
    ${HEADER_FILES}
	${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES}
)

add_executable(ConstructorTest
    ${TEST_FILES}
)

# set include directories
# set property for library 
set_property(
	TARGET Constructor 
	PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
)

#set property for the test executable
set_property(
	TARGET ConstructorTest 
	PROPERTY INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/include
	APPEND PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/include
)

# add required definitions (tr1 tuple is not supported under VS11)
set_target_properties(ConstructorTest         PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)

add_dependencies(ConstructorTest Constructor gtest)
target_link_libraries(ConstructorTest Constructor gtest)


# files layout for all targets
source_group ("Headers"     		FILES ${HEADER_FILES})
source_group ("Sources"     		FILES ${SOURCE_FILES})
source_group ("Private headers" 	FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"       		FILES ${TEST_FILES})

# add tests
add_test(
	NAME    ConstructorUnitTest
	WORKING_DIRECTORY ConstructorTest_BINARY_DIR
	COMMAND ConstructorTest
)

