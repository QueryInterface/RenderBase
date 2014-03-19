cmake_minimum_required (VERSION 2.8.11)
#set(DX_DIR $ENV{DXSDK_DIR})

# Add libraries
#find_library(d3d9 d3d9)
#add_library(d3dx9 STATIC IMPORTED )
#if (ARCH_64)
#  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x64\\d3dx9.lib)
#else()
#  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x86\\d3dx9.lib)
#endif()

# Include directories
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/Utils/include)

check_apple_platform()
if(WIN32)
    if (${ARCH_64})
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_windows_x64/include)
    else()
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_windows_x86/include)
    endif()
elseif(OS_OSX)
    if (${ARCH_64})
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_macos_x64/include)
    else()
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_macos_x86/include)
    endif()
elseif(TARGET_OS_IPHONE)
    if (${ARCH_64})
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_ios_x64/include)
    else()
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_ios_x86/include)
    endif()
elseif(ANDROID)
    if (${ARCH_64})
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_android_x64/include)
    else()
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_android_x86/include)
    endif()
else()
    if (${ARCH_64})
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_linux_x64/include)
    else()
        include_directories(${3RDPARTY_DIR}/_unpack/qt/Qt_linux_x86/include)
    endif()
endif()


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

target_link_libraries(RenderBase Utils)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
source_group ("Private headers"     FILES ${PRIVATE_HEADER_FILES})
source_group ("Tests"               FILES ${TEST_FILES})
 