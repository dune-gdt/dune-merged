// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   René Fritze     (2017, 2019)
//   Tim Keil        (2018)
//   Tobias Leibner  (2018, 2020)

#include <dune/xt/test/main.hxx> // <- has to come first (includes the config.h)!

#include <dune/xt/test/la/eigensolver.hh>

{% for T_NAME, TESTMATRIXTYPE, TESTFIELDTYPE, TESTCOMPLEXMATRIXTYPE, TESTREALMATRIXTYPE in config.testtypes %}
struct EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}
: public EigenSolverTestForMatricesWithRealEigenvaluesAndVectors<{{TESTMATRIXTYPE}},
        {{TESTFIELDTYPE}},
        {{TESTCOMPLEXMATRIXTYPE}},
        {{TESTREALMATRIXTYPE}}>
{
  using BaseType = EigenSolverTest;
  using typename BaseType::MatrixType;
  using typename BaseType::ComplexMatrixType;
  using typename BaseType::EigenValuesType;

  EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}()
  {
    matrix_ = XT::Common::from_string<MatrixType>("[1 1; -1 1]");
    expected_eigenvalues_ = XT::Common::from_string<EigenValuesType>("[1+1i 1-1i]");
    all_matrices_and_expected_eigenvalues_and_vectors_are_computed_ = true;
  }

  using BaseType::all_matrices_and_expected_eigenvalues_and_vectors_are_computed_;
  using BaseType::matrix_;
  using BaseType::expected_eigenvalues_;
}; // struct EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}


TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, exports_correct_types)
{
  exports_correct_types();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, has_types_and_options)
{
  has_types_and_options();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, throws_on_broken_matrix_construction)
{
  throws_on_broken_matrix_construction();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, allows_broken_matrix_construction_when_checks_disabled)
{
  allows_broken_matrix_construction_when_checks_disabled();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, throws_on_inconsistent_given_options)
{
  throws_on_inconsistent_given_options();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, is_constructible)
{
  is_constructible();
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, gives_correct_eigenvalues)
{
  gives_correct_eigenvalues( { {"shifted_qr", /*we_expect_a_failure: */ "-1"} } );
}

TEST_F(EigenSolverForRealMatrixWithComplexEvs_{{T_NAME}}, gives_correct_eigendecomposition)
{
  gives_correct_eigendecomposition( { {"shifted_qr", /*we_expect_a_failure: */ "-1"} } );
}
{% endfor %}
