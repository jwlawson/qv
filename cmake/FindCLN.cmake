find_path(CLN_INCLUDE_DIR
  NAMES cln/number.h cln/integer.h
  DOC "CLN numeric math library include directory"
)
find_library(CLN_LIBRARY
  NAMES ginac
  DOC "CLN numeric math library"
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CLN
  REQUIRED_VARS CLN_LIBRARY CLN_INCLUDE_DIR
)
mark_as_advanced(CLN_INCLUDE_DIR CLN_LIBRARY CLN_FOUND)
if(CLN_FOUND)
  set(CLN_INCLUDE_DIRS ${CLN_INCLUDE_DIR})
  set(CLN_LIBRARIES ${CLN_LIBRARY})
  add_library(CLN::CLN IMPORTED UNKNOWN)
  set_target_properties(CLN::CLN PROPERTIES
    IMPORTED_LOCATION ${CLN_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${CLN_INCLUDE_DIR}
  )
endif()
