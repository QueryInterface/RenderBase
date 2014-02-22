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
    FontRendererImpl.cpp)

# Form executable
add_library(RenderBase SHARED
    ${INTERFACE_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)
add_executable(AppShell
    main.cpp)

target_link_libraries(RenderBase d3d9 d3dx9 freetype Utils)
target_link_libraries(AppShell RenderBase)
#add_dependencies(AppShell RenderBase)

# Organize projects in folders
set_property (GLOBAL PROPERTY   USE_FOLDERS ON)
set_property (TARGET AppShell   PROPERTY FOLDER "Executables")
set_property (TARGET RenderBase PROPERTY FOLDER "Libraries")
set_property (TARGET Utils      PROPERTY FODLER "Libraries")
set_property (TARGET freetype   PROPERTY FOLDER "3rdParty/freetype")
set_property (TARGET dist       PROPERTY FOLDER "3rdParty/freetype")

# Set output folder
set_target_properties(RenderBase    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(RenderBase    PROPERTIES COMPILE_FLAGS -DDEFINE_EXPORT)
set_target_properties(AppShell      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(freetype      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set_target_properties(Utils         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

# Create layout for files inside VS/XCode
source_group ("_Interfaces" FILES ${INTERFACE_FILES})
source_group ("Headers"     FILES ${HEADER_FILES})
source_group ("Sources"     FILES ${SOURCE_FILES} main.cpp)

# Post-build steps
file(COPY Media DESTINATION ${CMAKE_BINARY_DIR}/Binaries)
