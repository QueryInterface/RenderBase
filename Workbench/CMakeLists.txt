cmake_minimum_required(VERSION 2.8.11)
project(Workbench)

# Global variables
set(3RDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../3rdParty)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(ARCH_64 TRUE) 
else() 
    set(ARCH_64 FALSE) 
endif()

# Add projects
add_subdirectory(MakeShared)

# Organize projects in folders
set_property (GLOBAL PROPERTY         USE_FOLDERS ON)
#set_property (TARGET QTWindowTest     PROPERTY FOLDER "Executables")
# set_property (TARGET RenderBase       PROPERTY FOLDER "Libraries")
# set_property (TARGET Utils            PROPERTY FODLER "Libraries")
# set_property (TARGET gtest            PROPERTY FOLDER "3rdParty/gtest")
# set_property (TARGET Constructor      PROPERTY FOLDER "Libraries")
# set_property (TARGET ConstructorTest  PROPERTY FOLDER "UnitTests")

# Set output folder
#set_target_properties(QTWindowTest  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(MakeShared  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)