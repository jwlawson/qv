
function(qv_target)
  set(_options)
  set(_one_value_args TARGET)
  set(_multi_value_args PUBLIC_LIBRARIES
                        PRIVATE_LIBRARIES)
  cmake_parse_arguments(QV_TARGET "${_options}"
                                  "${_one_value_args}"
                                  "${_multi_value_args}"
                                  ${ARGN})
  target_include_directories(${QV_TARGET_TARGET}
    PRIVATE ${qv_SOURCE_DIR}/include)
  target_compile_features(${QV_TARGET_TARGET} PRIVATE
    cxx_defaulted_functions
    cxx_deleted_functions
  )
  target_link_libraries(${QV_TARGET_TARGET}
    PUBLIC  ${QV_TARGET_PUBLIC_LIBRARIES}
    PRIVATE ${QV_TARGET_PRIVATE_LIBRARIES}
  )
endfunction()

function(qv_library)
  set(_options)
  set(_one_value_args TARGET)
  set(_multi_value_args SOURCES
                        PUBLIC_LIBRARIES
                        PRIVATE_LIBRARIES)
  cmake_parse_arguments(QV_LIB "${_options}"
                                "${_one_value_args}"
                                "${_multi_value_args}"
                                ${ARGN})
  add_library(${QV_LIB_TARGET} ${QV_LIB_SOURCES})
  qv_target(TARGET            ${QV_LIB_TARGET}
            PUBLIC_LIBRARIES  ${QV_LIB_PUBLIC_LIBRARIES}
            PRIVATE_LIBRARIES ${QV_LIB_PRIVATE_LIBRARIES})
endfunction()


function(qv_executable)
  set(_options)
  set(_one_value_args TARGET)
  set(_multi_value_args SOURCES
                        PUBLIC_LIBRARIES
                        PRIVATE_LIBRARIES)
  cmake_parse_arguments(QV_EXEC "${_options}"
                                "${_one_value_args}"
                                "${_multi_value_args}"
                                ${ARGN})
  add_executable(${QV_EXEC_TARGET} ${QV_EXEC_SOURCES})
  qv_target(TARGET            ${QV_EXEC_TARGET}
            PUBLIC_LIBRARIES  ${QV_EXEC_PUBLIC_LIBRARIES}
            PRIVATE_LIBRARIES ${QV_EXEC_PRIVATE_LIBRARIES})
endfunction()

function(qv_test)
  set(_options)
  set(_one_value_args TARGET)
  set(_multi_value_args SOURCES
                        PUBLIC_LIBRARIES)
  cmake_parse_arguments(QV_TEST "${_options}"
                                "${_one_value_args}"
                                "${_multi_value_args}"
                                ${ARGN})
  message(STATUS "Test target: ${QV_TEST_TARGET}")
  qv_executable(TARGET            ${QV_TEST_TARGET}
                SOURCES           ${QV_TEST_SOURCES}
                PUBLIC_LIBRARIES  ${QV_TEST_PUBLIC_LIBRARIES}
                PRIVATE_LIBRARIES GTest::GTest GTest::Main qv)
  add_test(NAME    ${QV_TEST_TARGET}
           COMMAND ${QV_TEST_TARGET})
endfunction()


