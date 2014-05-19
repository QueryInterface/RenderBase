disable_warning_level()

set(SDL_STATIC OFF)

add_subdirectory(gtest-1.6.0)
add_subdirectory(_unpack/SDL/SDL2)
add_subdirectory(_unpack/tinyobjloader)

set_property (TARGET gtest            PROPERTY FOLDER "3rdParty/gtest")
set_property (TARGET SDL2             PROPERTY FOLDER "3rdParty/SDL")
set_property (TARGET SDL2main         PROPERTY FOLDER "3rdParty/SDL")
if (SDL_STATIC)
    set_property (TARGET SDL2-static      PROPERTY FOLDER "3rdParty/SDL")
endif()
set_property (TARGET tinyobjloader    PROPERTY FOLDER "3rdParty/tinyobjloader")