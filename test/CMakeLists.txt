
qv_test(TARGET class_ext_iterator_test
        SOURCES class_ext_iterator_test.cc
)
qv_test(TARGET cycle_info_test
        SOURCES cycle_info_test.cc
)
qv_test(TARGET cycle_test
        SOURCES cycle_test.cc
)
qv_test(TARGET dynkin_test
        SOURCES dynkin_test.cc
)
qv_test(TARGET equiv_mutation_class_size_test
        SOURCES equiv_mutation_class_size_test.cc
)
qv_test(TARGET equiv_quiver_matrix_test
        SOURCES equiv_quiver_matrix_test.cc
)
qv_test(TARGET equiv_underlying_graph_test
        SOURCES equiv_underlying_graph_test.cc
)
qv_test(TARGET exchange_graph_test
        SOURCES exchange_graph_test.cc
)
qv_test(TARGET extension_iterator_test
        SOURCES extension_iterator_test.cc
)
qv_test(TARGET fast_infinite_test
        SOURCES fast_infinite_test.cc
)
qv_test(TARGET fast_mmi_test
        SOURCES fast_mmi_test.cc
)
qv_test(TARGET green_exchange_graph_test
        SOURCES green_exchange_graph_test.cc
)
qv_test(TARGET int_matrix_test
        SOURCES int_matrix_test.cc
)
qv_test(TARGET mass_finite_check_test
        SOURCES mass_finite_check_test.cc
)
qv_test(TARGET mmi_move_test
        SOURCES mmi_move_test.cc
)
qv_test(TARGET move_class_loader_test
        SOURCES move_class_loader_test.cc
)
qv_test(TARGET move_graph_test
        SOURCES move_graph_test.cc
)
qv_test(TARGET mutation_class_loader_test
        SOURCES mutation_class_loader_test.cc
)
qv_test(TARGET mutation_class_size_test
        SOURCES mutation_class_size_test.cc
)
qv_test(TARGET mutation_class_test
        SOURCES mutation_class_test.cc
)
qv_test(TARGET oriented_cycle_info_test
        SOURCES oriented_cycle_info_test.cc
)
qv_test(TARGET quiver_matrix_test
        SOURCES quiver_matrix_test.cc
)
qv_test(TARGET seed_test
        SOURCES seed_test.cc
)
qv_test(TARGET sink_source_graph_test
        SOURCES sink_source_graph_test.cc
)
qv_test(TARGET sized_submatrix_iterator_test
        SOURCES sized_submatrix_iterator_test.cc
)
qv_test(TARGET skew_symmetric_iterator_test
        SOURCES skew_symmetric_iterator_test.cc
)
qv_test(TARGET slow_mmi_test
        SOURCES slow_mmi_test.cc
)
qv_test(TARGET ss_move_test
        SOURCES ss_move_test.cc
)
qv_test(TARGET stream_extension_iterator_test
        SOURCES stream_extension_iterator_test.cc
)
qv_test(TARGET stream_iterator_test
        SOURCES stream_iterator_test.cc
)
# stream_iterator_test requires an empty file to open
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty "")
qv_test(TARGET stream_submatrix_iterator_test
        SOURCES stream_submatrix_iterator_test.cc
)
qv_test(TARGET submatrix_iterator_test
        SOURCES submatrix_iterator_test.cc
)
qv_test(TARGET underlying_graph_test
        SOURCES underlying_graph_test.cc
)
