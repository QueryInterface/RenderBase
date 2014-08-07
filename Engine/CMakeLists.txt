cmake_minimum_required (VERSION 2.8.11)

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES          ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB PRIVATE_HEADER_FILES  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB SOURCE_FILES          ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB SHADER_FILES          ${CMAKE_CURRENT_LIST_DIR}/shaders/*.shader)
file(GLOB TEST_FILES            ${CMAKE_CURRENT_LIST_DIR}/test/*.*)

set(TEST_DEP_FILES 
    ${CMAKE_CURRENT_LIST_DIR}/src/SceneElementImpl.cpp)

# Generate headers from shader files
foreach(file ${SHADER_FILES})
    add_custom_command(
        OUTPUT "${file}.h"
        COMMAND ${BIN2CPP} "${file}" "${file}.h"
        DEPENDS "${file}"
    )
    # bin2cpp("${file}", "${file}.h")
    message(STATUS "Processing: ${file}")
endforeach()
# Add generated files
foreach(file ${SHADER_FILES})
    list(APPEND SHADER_FILES "${file}.h")
endforeach()

message(STATUS "GGG: ${SHADER_FILES}")

add_executable(EngineTest
    ${TEST_FILES}
    ${TEST_DEP_FILES}
)

# Form binaries
add_library(Engine SHARED
    ${HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES}
    ${SHADER_FILES}
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
target_link_libraries(Engine ${EXTRA_LIBS})
# Definitions
add_definitions(-DDEFINE_EXPORT)
# Dependencies
if (WIN32)
	add_dependencies(Engine copy_angle_libraries
							copy_media_folder)
endif()

# Include directories for Engine and tests
include_directories(include)
include_directories(shaders)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/Utils/include)
include_directories(${SDL_DIR}/include)
include_directories(${GLM_DIR})

# Include directories for test executable
target_include_directories(EngineTest PUBLIC ${gtest_SOURCE_DIR}/include)

# add required definitions (tr1 tuple is not supported under VS11)
set_target_properties(EngineTest         PROPERTIES COMPILE_FLAGS -DGTEST_HAS_TR1_TUPLE=0)

target_link_libraries(EngineTest gtest Utils)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
source_group ("Shaders"             FILES ${SHADER_FILES})
source_group ("Private headers"     FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"               FILES ${TEST_FILES})

# Place porject in correct filter folder
set_property (TARGET Engine         PROPERTY FOLDER "Libraries")  
set_property (TARGET EngineTest     PROPERTY FOLDER "UnitTests")  

# add tests
add_test(
    NAME    EngineTest
    WORKING_DIRECTORY EngineTest_BINARY_DIR
    COMMAND EngineTest
)