cmake_minimum_required(VERSION 2.8.11)
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
project(RenderBase)

include(cmake/useful_functions.cmake)
update_compiler_settings()
check_apple_platform()

# Global variables
set(3RDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdParty)
set(SDL_DIR ${3RDPARTY_DIR}/_unpack/SDL/SDL2)
set(ANGLE_DIR ${3RDPARTY_DIR}/angle)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(ARCH_64 TRUE) 
else() 
    set(ARCH_64 FALSE) 
endif()

if (WIN32)
    set (DYNAMIC_LIB_SUFIX .dll)
    set (STATIC_LIB_SUFIX .lib)
elseif(APPLE)
    set (DYNAMIC_LIB_SUFIX .dylib)
    set (STATIC_LIB_SUFIX .a)
else()
    set (DYNAMIC_LIB_SUFIX .so)
    set (STATIC_LIB_SUFIX .a)    
endif()

# Configure WinSDK
if (WIN32)
    if(${ARCH_64})
        set (WINSDK_DIR ${3RDPARTY_DIR}/_unpack/WindowsKits/8.0/Lib/win8/um/x64)
    else()
        set (WINSDK_DIR ${3RDPARTY_DIR}/_unpack/WindowsKits/8.0/Lib/win8/um/x86)
    endif()
    set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WINSDK_DIR})
endif()

if (WIN32)
    add_definitions(-DSDL_VIDEO_OPENGL_EGL)
    add_definitions(-DSDL_VIDEO_OPENGL_ES2)
endif()

find_package(OpenGL REQUIRED)

# Set global build variables
set (ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# Set output folder
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add projects
add_subdirectory(3rdParty)
add_subdirectory(Engine)
add_subdirectory(AppShell)
add_subdirectory(Constructor)
add_subdirectory(Utils)

# Organize projects in folders
set_property (TARGET AppShell         PROPERTY FOLDER "Executables")
set_property (TARGET RenderBase       PROPERTY FOLDER "Libraries")

# Set additional targets' flags
set_target_properties(RenderBase    PROPERTIES COMPILE_FLAGS -DDEFINE_EXPORT)
set_target_properties(gtest         PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)

# set_target_properties(RenderBase    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
# set_target_properties(AppShell      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
# set_target_properties(freetype      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
# set_target_properties(Utils         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
# set_target_properties(gtest         PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

# Post-build steps
if (WIN32)
    add_custom_target(copy_angle_libraries
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANGLE_DIR}/bin/libEGL.dll           ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANGLE_DIR}/bin/libGLESv2.dll        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ANGLE_DIR}/bin/d3dcompiler_46.dll   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/
        COMMENT "Copying ANGLE binaries to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>" VERBATIM
    )
    set_property (TARGET copy_angle_libraries  PROPERTY FOLDER "3rdParty")
endif()    
