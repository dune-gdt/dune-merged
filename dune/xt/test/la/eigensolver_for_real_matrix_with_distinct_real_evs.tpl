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

constexpr int TESTMATRIXSIZE = 5;

{% for T_NAME, TESTMATRIXTYPE, TESTFIELDTYPE, TESTCOMPLEXMATRIXTYPE, TESTREALMATRIXTYPE in config.testtypes %}
struct EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}
: public EigenSolverTestForMatricesWithRealEigenvaluesAndVectors<{{TESTMATRIXTYPE}},
        {{TESTFIELDTYPE}},
        {{TESTCOMPLEXMATRIXTYPE}},
        {{TESTREALMATRIXTYPE}}>
{
  using BaseType = EigenSolverTestForMatricesWithRealEigenvaluesAndVectors;
  using typename BaseType::MatrixType;
  using typename BaseType::ComplexMatrixType;
  using typename BaseType::RealMatrixType;
  using typename BaseType::EigenValuesType;
  using typename BaseType::RealEigenValuesType;
  using typename BaseType::RealType;
  using typename BaseType::ComplexType;
  using typename BaseType::M;
  using ScalarType = typename M::S;

  EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}()
  {
    matrix_ = XT::LA::eye_matrix<MatrixType>(TESTMATRIXSIZE, TESTMATRIXSIZE);
    for (size_t ii = 0; ii < TESTMATRIXSIZE; ++ii)
      XT::Common::set_matrix_entry(matrix_, ii, ii, static_cast<ScalarType>(ii + 1));
    expected_eigenvalues_ = XT::Common::create<EigenValuesType>(TESTMATRIXSIZE);
    for (size_t ii = 0; ii < TESTMATRIXSIZE; ++ii)
      expected_eigenvalues_[ii] = static_cast<ComplexType>(ii + 1);
    expected_real_eigenvalues_ = XT::Common::create<RealEigenValuesType>(TESTMATRIXSIZE);
    for (size_t ii = 0; ii < TESTMATRIXSIZE; ++ii)
      expected_real_eigenvalues_[ii] = static_cast<RealType>(ii + 1);
    expected_max_ev_ = TESTMATRIXSIZE;
    expected_min_ev_ = 1;
    all_matrices_and_expected_eigenvalues_and_vectors_are_computed_ = true;
  }

  using BaseType::all_matrices_and_expected_eigenvalues_and_vectors_are_computed_;
  using BaseType::matrix_;
  using BaseType::expected_eigenvalues_;
  using BaseType::expected_real_eigenvalues_;
  using BaseType::expected_max_ev_;
  using BaseType::expected_min_ev_;
}; // struct EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}


TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, exports_correct_types)
{
  exports_correct_types();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, has_types_and_options)
{
  has_types_and_options();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, throws_on_broken_matrix_construction)
{
  throws_on_broken_matrix_construction();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, allows_broken_matrix_construction_when_checks_disabled)
{
  allows_broken_matrix_construction_when_checks_disabled();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, throws_on_inconsistent_given_options)
{
  throws_on_inconsistent_given_options();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, is_constructible)
{
  is_constructible();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_eigenvalues)
{
  gives_correct_eigenvalues();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_real_eigenvalues)
{
  gives_correct_real_eigenvalues();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_max_eigenvalue)
{
  gives_correct_max_eigenvalue();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_min_eigenvalue)
{
  gives_correct_min_eigenvalue();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_eigendecomposition)
{
  gives_correct_eigendecomposition();
}

TEST_F(EigenSolverForRowWiseScaledUniMatrix_{{T_NAME}}, gives_correct_real_eigendecomposition)
{
  gives_correct_real_eigendecomposition();
}
{% endfor %}
