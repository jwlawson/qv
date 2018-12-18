set(_header_path ${qv_SOURCE_DIR}/include)
set(_headers
  ${_header_path}/array_utils.h
  ${_header_path}/class_ext_iterator.h
  ${_header_path}/consts.h
  ${_header_path}/cycle.h
  ${_header_path}/cycle_info.h
  ${_header_path}/dynkin.h
  ${_header_path}/equivalence_checker.h
  ${_header_path}/equiv_mutation_class_loader.h
  ${_header_path}/equiv_mutation_class_size.h
  ${_header_path}/equiv_quiver_matrix.h
  ${_header_path}/equiv_underlying_graph.h
  ${_header_path}/exchange_graph.h
  ${_header_path}/extension_iterator.h
  ${_header_path}/fast_infinite.h
  ${_header_path}/ginac_util.h
  ${_header_path}/green_exchange_graph.h
  ${_header_path}/int_matrix.h
  ${_header_path}/link_holder.h
  ${_header_path}/mass_finite_check.h
  ${_header_path}/mmi.h
  ${_header_path}/mmi_move.h
  ${_header_path}/move_class_loader.h
  ${_header_path}/move_graph.h
  ${_header_path}/mutation_class.h
  ${_header_path}/mutation_class_loader.h
  ${_header_path}/mutation_class_size.h
  ${_header_path}/oriented_cycle_info.h
  ${_header_path}/quiver_matrix.h
  ${_header_path}/seed.h
  ${_header_path}/sink_source_graph.h
  ${_header_path}/sized_submatrix_iterator.h
  ${_header_path}/skew_symmetric_iterator.h
  ${_header_path}/ss_move.h
  ${_header_path}/stream_extension_iterator.h
  ${_header_path}/stream_iterator.h
  ${_header_path}/stream_submatrix_iterator.h
  ${_header_path}/submatrix_iterator.h
  ${_header_path}/template_exchange_graph.h
  ${_header_path}/template_exchange_graph.impl
  ${_header_path}/underlying_graph.h
)
set(_sources
  array_utils.cc
  class_ext_iterator.cc
  cycle.cc
  cycle_info.cc
  dynkin.cc
  equivalence_checker.cc
  equiv_mutation_class_loader.cc
  equiv_mutation_class_size.cc
  equiv_quiver_matrix.cc
  equiv_underlying_graph.cc
  extension_iterator.cc
  fast_infinite.cc
  int_matrix.cc
  link_holder.cc
  mass_finite_check.cc
  mmi.cc
  mmi_move.cc
  move_class_loader.cc
  move_graph.cc
  mutation_class.cc
  mutation_class_loader.cc
  mutation_class_size.cc
  oriented_cycle_info.cc
  quiver_matrix.cc
  sized_submatrix_iterator.cc
  skew_symmetric_iterator.cc
  ss_move.cc
  stream_extension_iterator.cc
  stream_iterator.cc
  stream_submatrix_iterator.cc
  submatrix_iterator.cc
  underlying_graph.cc
)
qv_library(
  TARGET  qv
  SOURCES ${_sources} ${_headers}
  PRIVATE_LIBRARIES Boost::boost
  INCLUDE_BINARY_DIR
)
add_library(QV::qv ALIAS qv)

include(GenerateExportHeader)
generate_export_header(qv)

include(GNUInstallDirs)
install(TARGETS qv
  EXPORT qv_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${_headers} ${CMAKE_CURRENT_BINARY_DIR}/qv_export.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qv
)
install(EXPORT qv_targets
  FILE QVConfig.cmake
  NAMESPACE QV::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QV
)
