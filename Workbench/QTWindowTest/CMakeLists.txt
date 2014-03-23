cmake_minimum_required (VERSION 2.8.11)

set (3RDPARTY_DIR ${PROJECT_SOURCE_DIR}/../3rdparty)
set (QT_DIR ${3RDPARTY_DIR}/_unpack/qt/Qt_windows_x86/)
set (WINSDK_DIR ${3RDPARTY_DIR}/_unpack/WindowsKits/8.0/Lib/win8/um/x86)

message(${WINSDK_DIR})

# Add libraries
set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR}/lib/cmake)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${QT_DIR}/lib/cmake)
set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${WINSDK_DIR})

find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(Qt5Widgets)
find_package(Qt5Declarative)
find_package(Qt5Quick)

#add_library(d3dx9 STATIC IMPORTED )
# if (ARCH_64)
#  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x64\\d3dx9.lib)
#else()
#  set_target_properties(d3dx9 PROPERTIES IMPORTED_LOCATION ${DX_DIR}Lib\\x86\\d3dx9.lib)
#endif()

# Include directories
include_directories(${QT_DIR}/include)

# list all header files from src and include folders automatically 
file(GLOB HEADER_FILES         ${CMAKE_CURRENT_LIST_DIR}/*.h)

# list all source files from src and test folders automatically 
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/  SOURCE_FILES)

# Form binaries
add_executable(QTWindowTest
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

qt5_use_modules(QTWindowTest Gui OpenGL Declarative Quick)
target_link_libraries(QTWindowTest)

#target_link_libraries(QTWindowTest qt5Gui qt5OpenGL)

# files layout for all targets
source_group ("Headers"             FILES ${HEADER_FILES})
source_group ("Sources"             FILES ${SOURCE_FILES})
 