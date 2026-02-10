#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mav_trajectory_generation::mav_trajectory_generation" for configuration ""
set_property(TARGET mav_trajectory_generation::mav_trajectory_generation APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(mav_trajectory_generation::mav_trajectory_generation PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/mav_trajectory_generation/libmav_trajectory_generation.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS mav_trajectory_generation::mav_trajectory_generation )
list(APPEND _IMPORT_CHECK_FILES_FOR_mav_trajectory_generation::mav_trajectory_generation "${_IMPORT_PREFIX}/lib/mav_trajectory_generation/libmav_trajectory_generation.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
