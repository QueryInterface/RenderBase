cmake_minimum_required (VERSION 2.8.11)

# Include directories
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/Utils/include)

include_directories(${SDL_DIR}/include)

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB PRIVATE_HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  SOURCE_FILES)

# Form binaries
add_library(RenderBase SHARED
    ${HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES}
)

# Add libraries
if (WIN32)
    set(DX_DIR $ENV{DXSDK_DIR})
    # dxerr for SDL
    find_library(dxerr dxerr)
    add_library(dxerr STATIC IMPORTED)
    if (ARCH_64)
        set_target_properties(dxerr PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib/x64/dxerr.lib)
    else()
        set_target_properties(dxerr PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib/x86/dxerr.lib)
    endif()
    list(APPEND EXTRA_LIBS dxerr)
    # angle for SDL
    find_library(libEGL libEGL)
    add_library(libEGL STATIC IMPORTED)
    set_target_properties(libEGL PROPERTIES IMPORTED_LOCATION ${ANGLE_DIR}/lib/libEGL.lib)
    find_library(libGLESv2 libGLESv2)
    add_library(libGLESv2 STATIC IMPORTED)
    set_target_properties(libGLESv2 PROPERTIES IMPORTED_LOCATION ${ANGLE_DIR}/lib/libGLESv2.lib)

    list(APPEND EXTRA_LIBS dxerr)   
    list(APPEND EXTRA_LIBS libEGL)   
    list(APPEND EXTRA_LIBS libGLESv2)
else()
    list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})
endif()
list(APPEND EXTRA_LIBS SDL2)
list(APPEND EXTRA_LIBS Utils)
target_link_libraries(RenderBase ${EXTRA_LIBS})

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
source_group ("Private headers"     FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"               FILES ${TEST_FILES}) 