cmake_minimum_required(VERSION 2.8.11)
project(RenderBase)

include(cmake/useful_functions.cmake)
update_compiler_settings(${PROJECT_SOURCE_DIR}/Engine/include)

# Global variables
set(3RDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdParty)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(ARCH_64 TRUE) 
else() 
    set(ARCH_64 FALSE) 
endif()

# Add projects
add_subdirectory(3rdParty)
add_subdirectory(Engine)
add_subdirectory(AppShell)
add_subdirectory(Constructor)
add_subdirectory(Utils)

# Organize projects in folders
set_property (GLOBAL PROPERTY         USE_FOLDERS ON)
set_property (TARGET AppShell         PROPERTY FOLDER "Executables")
set_property (TARGET RenderBase       PROPERTY FOLDER "Libraries")
set_property (TARGET Utils            PROPERTY FODLER "Libraries")
set_property (TARGET gtest            PROPERTY FOLDER "3rdParty/gtest")
set_property (TARGET Constructor      PROPERTY FOLDER "Libraries")
set_property (TARGET ConstructorTest  PROPERTY FOLDER "UnitTests")

# Set output folder
set_target_properties(RenderBase    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(RenderBase    PROPERTIES COMPILE_FLAGS -DDEFINE_EXPORT)
set_target_properties(AppShell      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(freetype      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(Utils         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(gtest         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(gtest         PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)

# Post-build steps
file(COPY Media DESTINATION ${CMAKE_BINARY_DIR}/Binaries)
