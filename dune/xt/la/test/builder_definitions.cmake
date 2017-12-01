set(DXT_BIN_COUNT "2" CACHE STRING "number of bins for test targets" )
add_custom_target(test_binaries_builder_0 DEPENDS headercheck__dune_xt_la_algorithms_cholesky.hh headercheck__dune_xt_la_algorithms_triangular_solves.hh headercheck__dune_xt_la_container.bindings.hh headercheck__dune_xt_la_container.hh headercheck__dune_xt_la_container_common.hh headercheck__dune_xt_la_container_common_matrix_sparse.hh headercheck__dune_xt_la_container_common_vector_dense.hh headercheck__dune_xt_la_container_common_vector_sparse.hh headercheck__dune_xt_la_container_container-interface.hh headercheck__dune_xt_la_container_eigen_base.hh headercheck__dune_xt_la_container_eigen_dense.hh headercheck__dune_xt_la_container_vector-interface.hh headercheck__dune_xt_la_eigen-solver.hh headercheck__dune_xt_la_eigen-solver_internal_base.hh headercheck__dune_xt_la_eigen-solver_internal_lapacke.hh headercheck__dune_xt_la_matrix-inverter.hh headercheck__dune_xt_la_matrix-inverter_fmatrix.hh headercheck__dune_xt_la_matrix-inverter_internal_base.hh headercheck__dune_xt_la_solver.hh headercheck__dune_xt_la_solver_common.hh headercheck__dune_xt_la_solver_istl.hh test_container_CommonDenseMatrix_double test_container_CommonSparseMatrixCsr_complex test_container_CommonSparseMatrixCsr_double test_container_CommonSparseOrDenseMatrixCsc_double test_container_CommonSparseOrDenseMatrixCsr_complex test_container_CommonSparseVector_double test_container_EigenDenseMatrix_double test_container_EigenDenseVector_complex test_container_EigenRowMajorSparseMatrix_complex test_container_IstlDenseVector_complex test_container_IstlDenseVector_double test_container_IstlRowMajorSparseMatrix_complex test_container_matrix_CommonDenseMatrix_CommonDenseVector_complex test_container_matrix_CommonDenseMatrix_CommonDenseVector_double test_container_matrix_CommonDenseMatrix_CommonSparseVector_complex test_container_matrix_CommonSparseMatrixCsr_CommonDenseVector_complex test_container_matrix_CommonSparseMatrixCsr_CommonDenseVector_double test_container_matrix_CommonSparseMatrixCsr_CommonSparseVector_complex test_container_matrix_CommonSparseMatrixCsr_CommonSparseVector_double test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonDenseVector_double test_container_matrix_EigenDenseMatrix_EigenDenseVector_double test_container_matrix_EigenRowMajorSparseMatrix_EigenDenseVector_double test_container_matrix_IstlRowMajorSparseMatrix_IstlDenseVector_double test_container_vector_CommonDenseVector_complex test_container_vector_CommonDenseVector_double test_container_vector_EigenDenseVector_complex test_container_vector_IstlDenseVector_double test_eigensolver_for_matrix_from_eigens_example_fieldmatrix_double test_eigensolver_for_real_matrix_with_distinct_real_evs_eigendense_double test_eigensolver_for_real_matrix_with_distinct_real_evs_fieldmatrix_double test_eigensolver_for_real_matrix_with_real_evs_fieldmatrix_double test_solver_EigenDenseMatrix_EigenDenseVector_EigenDenseVector_complex test_solver_EigenDenseMatrix_EigenDenseVector_EigenMappedDenseVector_double test_solver_EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_complex)
set_tests_properties(test_container_CommonDenseMatrix_double_la_container_0002 test_container_CommonDenseMatrix_double_la_container_0003 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_CommonSparseMatrixCsr_complex_la_container_0018 test_container_CommonSparseMatrixCsr_complex_la_container_0019 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_CommonSparseMatrixCsr_double_la_container_0020 test_container_CommonSparseMatrixCsr_double_la_container_0021 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_CommonSparseOrDenseMatrixCsc_double_la_container_0024 test_container_CommonSparseOrDenseMatrixCsc_double_la_container_0025 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_CommonSparseOrDenseMatrixCsr_complex_la_container_0026 test_container_CommonSparseOrDenseMatrixCsr_complex_la_container_0027 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_CommonSparseVector_double_la_container_0035 test_container_CommonSparseVector_double_la_container_0036 test_container_CommonSparseVector_double_la_container_0037 test_container_CommonSparseVector_double_la_container_0038 test_container_CommonSparseVector_double_la_container_0039 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_EigenDenseMatrix_double_la_container_0041 test_container_EigenDenseMatrix_double_la_container_0042 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_EigenDenseVector_complex_la_container_0043 test_container_EigenDenseVector_complex_la_container_0044 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_EigenRowMajorSparseMatrix_complex_la_container_0049 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_IstlDenseVector_complex_la_container_0052 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_IstlDenseVector_double_la_container_0053 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_IstlRowMajorSparseMatrix_complex_la_container_0054 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonDenseMatrix_CommonDenseVector_complex_la_container_matrix_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonDenseMatrix_CommonDenseVector_double_la_container_matrix_0001 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonDenseMatrix_CommonSparseVector_complex_la_container_matrix_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsr_CommonDenseVector_complex_la_container_matrix_0008 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsr_CommonDenseVector_double_la_container_matrix_0009 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsr_CommonSparseVector_complex_la_container_matrix_0010 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsr_CommonSparseVector_double_la_container_matrix_0011 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonDenseVector_double_la_container_matrix_0013 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_EigenDenseMatrix_EigenDenseVector_double_la_container_matrix_0021 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_EigenRowMajorSparseMatrix_EigenDenseVector_double_la_container_matrix_0024 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_matrix_IstlRowMajorSparseMatrix_IstlDenseVector_double_la_container_matrix_0027 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_vector_CommonDenseVector_complex_la_container_vector_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_vector_CommonDenseVector_double_la_container_vector_0001 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_vector_EigenDenseVector_complex_la_container_vector_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_container_vector_IstlDenseVector_double_la_container_vector_0008 PROPERTIES LABELS "builder_0")
set_tests_properties(test_eigensolver_for_matrix_from_eigens_example_fieldmatrix_double_la_eigensolver_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_eigensolver_for_real_matrix_with_distinct_real_evs_eigendense_double_la_eigensolver_0001 PROPERTIES LABELS "builder_0")
set_tests_properties(test_eigensolver_for_real_matrix_with_distinct_real_evs_fieldmatrix_double_la_eigensolver_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_eigensolver_for_real_matrix_with_real_evs_fieldmatrix_double_la_eigensolver_0000 PROPERTIES LABELS "builder_0")
set_tests_properties(test_solver_EigenDenseMatrix_EigenDenseVector_EigenDenseVector_complex_la_solver_0002 PROPERTIES LABELS "builder_0")
set_tests_properties(test_solver_EigenDenseMatrix_EigenDenseVector_EigenMappedDenseVector_double_la_solver_0004 PROPERTIES LABELS "builder_0")
set_tests_properties(test_solver_EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_complex_la_solver_0007 PROPERTIES LABELS "builder_0")
add_custom_target(test_binaries_builder_1 DEPENDS headercheck__dune_xt_la_algorithms.hh headercheck__dune_xt_la_algorithms_qr.hh headercheck__dune_xt_la_algorithms_solve_sym_tridiag_posdef.hh headercheck__dune_xt_la_container_common_matrix.hh headercheck__dune_xt_la_container_common_matrix_dense.hh headercheck__dune_xt_la_container_common_vector.hh headercheck__dune_xt_la_container_conversion.hh headercheck__dune_xt_la_container_eigen.hh headercheck__dune_xt_la_container_eigen_sparse.hh headercheck__dune_xt_la_container_eye-matrix.hh headercheck__dune_xt_la_container_interfaces.hh headercheck__dune_xt_la_container_io.hh headercheck__dune_xt_la_container_istl.hh headercheck__dune_xt_la_container_matrix-interface.hh headercheck__dune_xt_la_container_pattern.hh headercheck__dune_xt_la_container_unit_matrices.hh headercheck__dune_xt_la_container_vector-interface-internal.hh headercheck__dune_xt_la_eigen-solver_eigen.hh headercheck__dune_xt_la_eigen-solver_fmatrix.hh headercheck__dune_xt_la_eigen-solver_internal_eigen.hh headercheck__dune_xt_la_eigen-solver_internal_numpy.hh headercheck__dune_xt_la_eigen-solver_internal_qrhouseholder.hh headercheck__dune_xt_la_exceptions.hh headercheck__dune_xt_la_matrix-inverter_eigen.hh headercheck__dune_xt_la_matrix-inverter_internal_eigen.hh headercheck__dune_xt_la_solver_eigen.hh headercheck__dune_xt_la_solver_fasp.hh headercheck__dune_xt_la_solver_istl_amg.hh headercheck__dune_xt_la_test_container.hh headercheck__dune_xt_la_test_eigensolver.hh test_container_CommonDenseMatrix_complex test_container_CommonDenseVector_complex test_container_CommonDenseVector_double test_container_CommonSparseMatrixCsc_complex test_container_CommonSparseMatrixCsc_double test_container_CommonSparseOrDenseMatrixCsc_complex test_container_CommonSparseOrDenseMatrixCsr_double test_container_CommonSparseVector_complex test_container_EigenDenseMatrix_complex test_container_EigenDenseVector_double test_container_EigenMappedDenseVector_double test_container_EigenRowMajorSparseMatrix_double test_container_IstlRowMajorSparseMatrix_double test_container_matrix_CommonDenseMatrix_CommonSparseVector_double test_container_matrix_CommonSparseMatrixCsc_CommonDenseVector_complex test_container_matrix_CommonSparseMatrixCsc_CommonDenseVector_double test_container_matrix_CommonSparseMatrixCsc_CommonSparseVector_complex test_container_matrix_CommonSparseMatrixCsc_CommonSparseVector_double test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonDenseVector_complex test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonSparseVector_complex test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonSparseVector_double test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonDenseVector_complex test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonDenseVector_double test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonSparseVector_complex test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonSparseVector_double test_container_matrix_EigenDenseMatrix_EigenDenseVector_complex test_container_matrix_EigenDenseMatrix_EigenMappedDenseVector_double test_container_matrix_EigenRowMajorSparseMatrix_EigenDenseVector_complex test_container_matrix_EigenRowMajorSparseMatrix_EigenMappedDenseVector_double test_container_matrix_IstlRowMajorSparseMatrix_IstlDenseVector_complex test_container_vector_CommonSparseVector_complex test_container_vector_CommonSparseVector_double test_container_vector_EigenDenseVector_double test_container_vector_EigenMappedDenseVector_double test_container_vector_IstlDenseVector_complex test_eigensolver_for_matrix_from_eigens_example_eigendense_double test_eigensolver_for_real_matrix_with_complex_evs_eigendense_double test_eigensolver_for_real_matrix_with_complex_evs_fieldmatrix_double test_eigensolver_for_real_matrix_with_real_evs_eigendense_double test_eigensolver_for_real_matrix_with_real_evs_from_2d_euler_equations_eigendense_double test_eigensolver_for_real_matrix_with_real_evs_from_2d_euler_equations_fieldmatrix_double test_empty test_solver_CommonDenseMatrix_CommonDenseVector_CommonDenseVector_complex test_solver_CommonDenseMatrix_CommonDenseVector_CommonDenseVector_double test_solver_EigenDenseMatrix_EigenDenseVector_EigenDenseVector_double test_solver_EigenDenseMatrix_EigenMappedDenseVector_EigenDenseVector_double test_solver_EigenDenseMatrix_EigenMappedDenseVector_EigenMappedDenseVector_double test_solver_EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_double)
set_tests_properties(test_container_CommonDenseMatrix_complex_la_container_0000 test_container_CommonDenseMatrix_complex_la_container_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonDenseVector_complex_la_container_0004 test_container_CommonDenseVector_complex_la_container_0005 test_container_CommonDenseVector_complex_la_container_0006 test_container_CommonDenseVector_complex_la_container_0007 test_container_CommonDenseVector_complex_la_container_0008 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonDenseVector_double_la_container_0009 test_container_CommonDenseVector_double_la_container_0010 test_container_CommonDenseVector_double_la_container_0011 test_container_CommonDenseVector_double_la_container_0012 test_container_CommonDenseVector_double_la_container_0013 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonSparseMatrixCsc_complex_la_container_0014 test_container_CommonSparseMatrixCsc_complex_la_container_0015 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonSparseMatrixCsc_double_la_container_0016 test_container_CommonSparseMatrixCsc_double_la_container_0017 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonSparseOrDenseMatrixCsc_complex_la_container_0022 test_container_CommonSparseOrDenseMatrixCsc_complex_la_container_0023 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonSparseOrDenseMatrixCsr_double_la_container_0028 test_container_CommonSparseOrDenseMatrixCsr_double_la_container_0029 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_CommonSparseVector_complex_la_container_0030 test_container_CommonSparseVector_complex_la_container_0031 test_container_CommonSparseVector_complex_la_container_0032 test_container_CommonSparseVector_complex_la_container_0033 test_container_CommonSparseVector_complex_la_container_0034 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_EigenDenseMatrix_complex_la_container_0040 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_EigenDenseVector_double_la_container_0045 test_container_EigenDenseVector_double_la_container_0046 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_EigenMappedDenseVector_double_la_container_0047 test_container_EigenMappedDenseVector_double_la_container_0048 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_EigenRowMajorSparseMatrix_double_la_container_0050 test_container_EigenRowMajorSparseMatrix_double_la_container_0051 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_IstlRowMajorSparseMatrix_double_la_container_0055 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonDenseMatrix_CommonSparseVector_double_la_container_matrix_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsc_CommonDenseVector_complex_la_container_matrix_0004 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsc_CommonDenseVector_double_la_container_matrix_0005 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsc_CommonSparseVector_complex_la_container_matrix_0006 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseMatrixCsc_CommonSparseVector_double_la_container_matrix_0007 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonDenseVector_complex_la_container_matrix_0012 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonSparseVector_complex_la_container_matrix_0014 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsc_CommonSparseVector_double_la_container_matrix_0015 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonDenseVector_complex_la_container_matrix_0016 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonDenseVector_double_la_container_matrix_0017 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonSparseVector_complex_la_container_matrix_0018 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_CommonSparseOrDenseMatrixCsr_CommonSparseVector_double_la_container_matrix_0019 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_EigenDenseMatrix_EigenDenseVector_complex_la_container_matrix_0020 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_EigenDenseMatrix_EigenMappedDenseVector_double_la_container_matrix_0022 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_EigenRowMajorSparseMatrix_EigenDenseVector_complex_la_container_matrix_0023 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_EigenRowMajorSparseMatrix_EigenMappedDenseVector_double_la_container_matrix_0025 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_matrix_IstlRowMajorSparseMatrix_IstlDenseVector_complex_la_container_matrix_0026 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_vector_CommonSparseVector_complex_la_container_vector_0002 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_vector_CommonSparseVector_double_la_container_vector_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_vector_EigenDenseVector_double_la_container_vector_0005 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_vector_EigenMappedDenseVector_double_la_container_vector_0006 PROPERTIES LABELS "builder_1")
set_tests_properties(test_container_vector_IstlDenseVector_complex_la_container_vector_0007 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_matrix_from_eigens_example_eigendense_double_la_eigensolver_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_real_matrix_with_complex_evs_eigendense_double_la_eigensolver_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_real_matrix_with_complex_evs_fieldmatrix_double_la_eigensolver_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_real_matrix_with_real_evs_eigendense_double_la_eigensolver_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_real_matrix_with_real_evs_from_2d_euler_equations_eigendense_double_la_eigensolver_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_eigensolver_for_real_matrix_with_real_evs_from_2d_euler_equations_fieldmatrix_double_la_eigensolver_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_empty PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_CommonDenseMatrix_CommonDenseVector_CommonDenseVector_complex_la_solver_0000 PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_CommonDenseMatrix_CommonDenseVector_CommonDenseVector_double_la_solver_0001 PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_EigenDenseMatrix_EigenDenseVector_EigenDenseVector_double_la_solver_0003 PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_EigenDenseMatrix_EigenMappedDenseVector_EigenDenseVector_double_la_solver_0005 PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_EigenDenseMatrix_EigenMappedDenseVector_EigenMappedDenseVector_double_la_solver_0006 PROPERTIES LABELS "builder_1")
set_tests_properties(test_solver_EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_double_la_solver_0008 PROPERTIES LABELS "builder_1")
