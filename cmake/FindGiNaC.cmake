find_path(GINAC_INCLUDE_DIR
  NAMES ginac/ginac.h
  DOC "GiNaC symbolic math library include directory"
)
find_library(GINAC_LIBRARY
  NAMES ginac
  DOC "GiNaC symbolic math library"
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GINAC
  REQUIRED_VARS GINAC_LIBRARY GINAC_INCLUDE_DIR
)
mark_as_advanced(GINAC_INCLUDE_DIR GINAC_LIBRARY GINAC_FOUND)
if(GINAC_FOUND)
  find_package(CLN REQUIRED)
  set(GINAC_INCLUDE_DIRS ${GINAC_INCLUDE_DIR})
  set(GINAC_LIBRARIES ${GINAC_LIBRARY})
  add_library(GiNaC::GiNaC IMPORTED UNKNOWN)
  set_target_properties(GiNaC::GiNaC PROPERTIES
    IMPORTED_LOCATION ${GINAC_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${GINAC_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES CLN::CLN
    INTERFACE_COMPILE_DEFINITIONS QV_USE_GINAC
  )
endif()
