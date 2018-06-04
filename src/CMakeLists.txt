qv_library(
  TARGET  qv
  SOURCES array_utils.cc
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
  PUBLIC_LIBRARIES GiNaC::GiNaC
                   Boost::boost
)
