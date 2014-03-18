# the file contains useful macros for cmake build

# set MT property for C++ runtime
macro(force_multithreaded)
  if (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

# increase warning level to W4
macro(force_warning_level)
  if (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      string(REPLACE "/W3" "-W4" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

# set all compiler properties (W4 and MT)
macro(update_compiler_settings)
  if (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
	  string(REPLACE "/W3" "-W4" ${flag_var} "${${flag_var}}")
    endforeach()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
  endif()
endmacro()