#.rst:
# FindAcbAPI
# --------
# Finds the AcbAPI library
#
# This will define the following variables::
#
# ACBAPI_FOUND - system has AcbAPI
# ACBAPI_INCLUDE_DIRS - the AcbAPI include directory
# ACBAPI_LIBRARIES - the AcbAPI libraries
# ACBAPI_DEFINITIONS - the AcbAPI compile definitions
#
# and the following imported targets::
#
#   ACBAPI::ACBAPI   - The AcbAPI library

if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_ACBAPI libAcbAPI>=1.0.0 QUIET)
endif()

find_path(ACBAPI_INCLUDE_DIR NAMES appswitching-control-block/AcbAPI.h
        PATHS ${PC_ACBAPI_INCLUDEDIR})
find_library(ACBAPI_LIBRARY NAMES AcbAPI
        PATHS ${PC_ACBAPI_LIBDIR})

set(ACBAPI_VERSION 1.0.0)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AcbAPI
                                  REQUIRED_VARS ACBAPI_LIBRARY ACBAPI_INCLUDE_DIR
                                  VERSION_VAR ACBAPI_VERSION)

if(ACBAPI_FOUND)
  set(ACBAPI_INCLUDE_DIRS ${ACBAPI_INCLUDE_DIR})
  set(ACBAPI_LIBRARIES ${ACBAPI_LIBRARY})

  if(NOT TARGET ACBAPI::ACBAPI)
    add_library(ACBAPI::ACBAPI UNKNOWN IMPORTED)
    set_target_properties(ACBAPI::ACBAPI PROPERTIES
                                     IMPORTED_LOCATION "${ACBAPI_LIBRARY}"
                                     INTERFACE_INCLUDE_DIRECTORIES "${ACBAPI_INCLUDE_DIR}")
  endif()
endif()

mark_as_advanced(ACBAPI_INCLUDE_DIR ACBAPI_LIBRARY)
