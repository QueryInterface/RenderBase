cmake_minimum_required(VERSION 2.8.11)
project(RenderBase)

# Global variables
set(3RDPARTY_DIR 3rdParty)

# Add projects
add_subdirectory(3rdParty)
add_subdirectory(Utils)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(ARCH_64 TRUE) 
else() 
    set(ARCH_64 FALSE) 
endif()

set(DX_DIR $ENV{DXSDK_DIR})

# Add libraries
find_library(d3d9 d3d9)
add_library(d3dx9 STATIC IMPORTED )
if (ARCH_64)
  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x64\\d3dx9.lib)
else()
  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x86\\d3dx9.lib)
endif()

# Include directories
include_directories(SYSTEM ${DX_DIR}\\Include)
include_directories(${3RDPARTY_DIR}/freetype/include)
include_directories(Includes)
include_directories(Utils)

# Define source groups
set (INTERFACE_FILES
    Includes/RenderContext.h
    Includes/Sprite.h
    Includes/FontRenderer.h)

set (HEADER_FILES
    Includes/common.h
    TypeAdapter.h
    RenderContextImpl.h
    SpriteImpl.h
    FontRendererImpl.h)

set (SOURCE_FILES
    RenderContextImpl.cpp
    SpriteImpl.cpp
    FontRendererImpl.cpp
    main.cpp)

# Form executable
add_executable(MeshRenderer
    ${INTERFACE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(MeshRenderer d3d9 d3dx9 freetype Utils)

# Organize projects in folders
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
set_property (TARGET MeshRenderer PROPERTY FOLDER "Executables")
set_property (TARGET Utils      PROPERTY FODLER "Libraries")
set_property (TARGET freetype   PROPERTY FOLDER "3rdParty/freetype")
set_property (TARGET dist     PROPERTY FOLDER "3rdParty/freetype")

# Set output folder
set_target_properties(MeshRenderer  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(freetype      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(Utils         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

# Create layout for files inside VS/XCode
source_group ("_Interfaces"  FILES ${INTERFACE_FILES})
source_group ("Headers"     FILES ${HEADER_FILES})
source_group ("Sources"     FILES ${SOURCE_FILES})

# Post-build steps
file(COPY Media DESTINATION ${CMAKE_BINARY_DIR}/Binaries)
