// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)
//   René Fritze     (2016 - 2019)
//   Tobias Leibner  (2015, 2017, 2019 - 2021)

#include <dune/xt/test/main.hxx>

#include <dune/xt/test/la/container.hh>
#include <dune/xt/test/common/float_cmp.hh>

using namespace Dune;
using namespace Dune::XT;

static constexpr size_t dim = 4;

{% for T_NAME, M_TYPE, V_TYPE in config.testtypes %}
struct MatrixTest_{{T_NAME}} : public ::testing::Test
{
  using MatrixImp = {{M_TYPE}};
  using VectorImp = {{V_TYPE}};
  using PatternType = Dune::XT::LA::SparsityPatternDefault;

  void fulfills_interface() const
  {
    // static tests
    using Traits = typename MatrixImp::Traits;
    // * of the traits
    using T_derived_type = typename Traits::derived_type;
    static_assert(std::is_same<MatrixImp, T_derived_type>::value, "derived_type has to be the correct Type!");
    using T_ScalarType = typename Traits::ScalarType;
    using T_RealType = typename Traits::RealType;
    // * of the matrix as itself (aka the derived type)
    using D_ScalarType = typename MatrixImp::ScalarType;
    using D_RealType = typename MatrixImp::RealType;
    static_assert(std::is_same<T_ScalarType, D_ScalarType>::value,
                  "ScalarType of derived_type has to be the correct Type!");
    static_assert(std::is_same<T_RealType, D_RealType>::value, "RealType of derived_type has to be the correct Type!");
    // * of the matrix as the interface
    using InterfaceType = typename XT::LA::MatrixInterface<Traits, D_ScalarType>;
    using I_derived_type = typename InterfaceType::derived_type;
    using I_ScalarType = typename InterfaceType::ScalarType;
    using I_RealType = typename InterfaceType::RealType;
    static_assert(std::is_same<MatrixImp, I_derived_type>::value, "derived_type has to be the correct Type!");
    static_assert(std::is_same<T_ScalarType, I_ScalarType>::value,
                  "ScalarType of derived_type has to be the correct Type!");
    static_assert(std::is_same<T_RealType, I_RealType>::value, "RealType of derived_type has to be the correct Type!");
    // dynamic tests
    // * of the matrix as itself (aka the derived type)
    MatrixImp d_by_size(dim, dim);
    PatternType pattern(dim);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        pattern.inner(ii).push_back(jj);
    }
    MatrixImp d_by_size_and_pattern(dim, dim, pattern);
    size_t d_rows = d_by_size.rows();
    EXPECT_EQ(dim, d_rows);
    size_t d_cols = d_by_size.cols();
    EXPECT_EQ(dim, d_cols);
    d_rows = d_by_size_and_pattern.rows();
    EXPECT_EQ(dim, d_rows);
    d_cols = d_by_size_and_pattern.cols();
    EXPECT_EQ(dim, d_cols);
    VectorImp zeros(dim);
    VectorImp ones(dim, D_ScalarType(1));
    VectorImp result(dim);
    d_by_size_and_pattern.mv(ones, result);
    EXPECT_EQ(zeros, result);
    for (size_t ii = 0; ii < d_rows; ++ii) {
      d_by_size_and_pattern.unit_row(ii);
      EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(1), d_by_size_and_pattern.get_entry(ii, ii));
      for (size_t jj = 0; jj < ii; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
      for (size_t jj = ii + 1; jj < d_cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
    }
    for (size_t ii = 0; ii < d_rows; ++ii) {
      d_by_size_and_pattern.clear_row(ii);
      for (size_t jj = 0; jj < d_cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
    }
    for (size_t jj = 0; jj < d_cols; ++jj) {
      d_by_size_and_pattern.unit_col(jj);
      EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(1), d_by_size_and_pattern.get_entry(jj, jj));
      for (size_t ii = 0; ii < jj; ++ii) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
      for (size_t ii = jj + 1; ii < d_rows; ++ii) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
    }
    for (size_t jj = 0; jj < d_cols; ++jj) {
      d_by_size_and_pattern.clear_col(jj);
      for (size_t ii = 0; ii < d_rows; ++ii) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(0), d_by_size_and_pattern.get_entry(ii, jj));
      }
    }
    for (size_t ii = 0; ii < d_rows; ++ii) {
      for (size_t jj = 0; jj < d_cols; ++jj) {
        d_by_size_and_pattern.set_entry(ii, jj, D_ScalarType(0.5 + ii + jj));
        d_by_size_and_pattern.add_to_entry(ii, jj, D_ScalarType(0.5 + ii + jj));
        EXPECT_DOUBLE_OR_COMPLEX_EQ(D_RealType(2 * ii + 2 * jj + 1), d_by_size_and_pattern.get_entry(ii, jj));
      }
    }
  } // void fulfills_interface() const

  void produces_correct_results() const
  {
    using ScalarType = typename MatrixImp::ScalarType;
    using RealType = typename MatrixImp::RealType;

    // create test patterns
    PatternType dense_pattern(dim);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        dense_pattern.inner(ii).push_back(jj);
    }
    PatternType sparse_pattern(dim);
    sparse_pattern.inner(0).push_back(2); //|-, -, x, -|
    sparse_pattern.inner(1).push_back(0); //|x, x, -, -|
    sparse_pattern.inner(1).push_back(1); //|-, -, -, x|
    sparse_pattern.inner(2).push_back(3); //|-, -, -, -|

    PatternType sparse_pattern2(dim);
    for (size_t jj = 0; jj < dim; ++jj)
      sparse_pattern2.inner(0).push_back(jj); //|x, x, x, x|
    sparse_pattern2.inner(1).push_back(0);    //|x, x, -, -|
    sparse_pattern2.inner(1).push_back(1);    //|x, -, x, -|
    sparse_pattern2.inner(2).push_back(0);    //|x, -, x, -|
    sparse_pattern2.inner(2).push_back(2);
    sparse_pattern2.inner(3).push_back(0);
    sparse_pattern2.inner(3).push_back(2);

    // create test matrizes
    MatrixImp matrix_zeros_dense(dim, dim, dense_pattern); // |0, 0, 0, 0|
    for (size_t ii = 0; ii < dim; ++ii) { // |0, 0, 0, 0|
      for (size_t jj = 0; jj < dim; ++jj) // |0, 0, 0, 0|
        matrix_zeros_dense.set_entry(ii, jj, ScalarType(0)); // |0, 0, 0, 0|
    }
    MatrixImp matrix_zeros_sparse(dim, dim, sparse_pattern); //|-, -, 0, -|
    matrix_zeros_sparse.set_entry(0, 2, ScalarType(0)); //|0, 0, -, -|
    matrix_zeros_sparse.set_entry(1, 0, ScalarType(0)); //|-, -, -, 0|
    matrix_zeros_sparse.set_entry(1, 1, ScalarType(0)); //|-, -, -, -|
    matrix_zeros_sparse.set_entry(2, 3, ScalarType(0));
    MatrixImp matrix_ones(dim, dim, dense_pattern); // |1, 1, 1, 1|
    for (size_t ii = 0; ii < dim; ++ii) { // |1, 1, 1, 1|
      for (size_t jj = 0; jj < dim; ++jj) // |1, 1, 1, 1|
        matrix_ones.set_entry(ii, jj, ScalarType(1)); // |1, 1, 1, 1|
    }
    MatrixImp testmatrix_1(dim, dim, dense_pattern); // |0, 1, 2, 3|
    for (size_t ii = 0; ii < dim; ++ii) { // |1, 2, 3, 4|
      for (size_t jj = 0; jj < dim; ++jj) // |2, 3, 4, 5|
        testmatrix_1.set_entry(ii, jj, ScalarType(ii) + ScalarType(jj)); // |3, 4, 5, 6|
    }
    MatrixImp testmatrix_2(dim, dim, dense_pattern); // | 0.0, 1.5, 3.0, 4.5|
    for (size_t ii = 0; ii < dim; ++ii) { // |-0.5, 1.0, 2.5, 4.0|
      for (size_t jj = 0; jj < dim; ++jj) // |-1.0, 0.5, 2.0, 3.5|
        testmatrix_2.set_entry(
            ii, jj, ScalarType(-0.5) * ScalarType(ii) + ScalarType(1.5) * ScalarType(jj)); // |-1.5, 0.0, 1.5, 3.0|
    }
    MatrixImp testmatrix_sparse(dim, dim, sparse_pattern); //|-,   -, 0.5,    -|
    testmatrix_sparse.set_entry(0, 2, ScalarType(0.5)); //|1, 1.5,   -,    -|
    testmatrix_sparse.set_entry(1, 0, ScalarType(1)); //|-,   -,   -, -0.5|
    testmatrix_sparse.set_entry(1, 1, ScalarType(1.5)); //|-,   -,   -,    -|
    testmatrix_sparse.set_entry(2, 3, ScalarType(-0.5));

    MatrixImp testmatrix_sparse2(dim, dim, sparse_pattern2);
    for (size_t ii = 0; ii < dim; ++ii)
      for (auto&& jj : sparse_pattern2.inner(ii))
        testmatrix_sparse2.set_entry(ii, jj, 2.);

    // create test vectors
    VectorImp vector_zeros(dim); // [0, 0, 0, 0]
    VectorImp vector_ones(dim, ScalarType(1)); // [1, 1, 1, 1]
    VectorImp vector_countingup(dim); //[0, 1, 2, 3]
    for (size_t ii = 0; ii < dim; ++ii)
      vector_countingup.set_entry(ii, ScalarType(ii));
    VectorImp testvector_1(dim); // [0, -2, 2, 1]
    testvector_1.set_entry(0, ScalarType(0));
    testvector_1.set_entry(1, ScalarType(-2));
    testvector_1.set_entry(2, ScalarType(2));
    testvector_1.set_entry(3, ScalarType(1));
    VectorImp testvector_3(dim); // [-1, 1, -1, 1]
    testvector_3.set_entry(0, ScalarType(-1));
    testvector_3.set_entry(1, ScalarType(1));
    testvector_3.set_entry(2, ScalarType(-1));
    testvector_3.set_entry(3, ScalarType(1));
    VectorImp testvector_5(dim); // [1.25, 0, 2.5, -3.5]
    testvector_5.set_entry(0, ScalarType(1.25));
    testvector_5.set_entry(1, ScalarType(0));
    testvector_5.set_entry(2, ScalarType(2.5));
    testvector_5.set_entry(3, ScalarType(-3.5));

    // test get_entry()
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), testmatrix_1.get_entry(0, 0));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(3), testmatrix_1.get_entry(1, 2));

    // test mv
    VectorImp result_mv_1(dim);
    matrix_zeros_dense.mv(vector_zeros, result_mv_1);
    VectorImp result_mv_2(dim);
    matrix_zeros_sparse.mv(vector_zeros, result_mv_2);
    EXPECT_EQ(vector_zeros, result_mv_1);
    EXPECT_EQ(vector_zeros, result_mv_2);
    testmatrix_sparse.mv(testvector_5, result_mv_1);
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(1.25), result_mv_1.get_entry(0));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(1.25), result_mv_1.get_entry(1));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(1.75), result_mv_1.get_entry(2));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), result_mv_1.get_entry(3));
    testmatrix_2.mv(testvector_3, result_mv_1);
    result_mv_2 = vector_ones;
    result_mv_2.scal(ScalarType(3));
    EXPECT_EQ(result_mv_1, result_mv_2);
    testmatrix_1.mv(testvector_1, result_mv_1);
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(5), result_mv_1.get_entry(0));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(6), result_mv_1.get_entry(1));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(7), result_mv_1.get_entry(2));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(8), result_mv_1.get_entry(3));
    testmatrix_sparse.mv(vector_ones, result_mv_1);
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0.5), result_mv_1.get_entry(0));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(2.5), result_mv_1.get_entry(1));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(-0.5), result_mv_1.get_entry(2));
    EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), result_mv_1.get_entry(3));
    VectorImp a = vector_ones;
    matrix_zeros_dense.mv(vector_zeros, a);
    for (size_t ii = 0; ii < dim; ++ii) {
      EXPECT_TRUE(Common::FloatCmp::eq(ScalarType(1), vector_ones.get_entry(ii))) << "check copy-on-write";
    }

    // test scal, operator*
    MatrixImp scaled = matrix_zeros_dense;
    MatrixImp scaled_by_operator = matrix_zeros_dense;
    size_t rows = scaled.rows();
    size_t cols = scaled.cols();
    scaled.scal(ScalarType(1));
    scaled_by_operator *= ScalarType(1);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), scaled.get_entry(ii, jj));
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), scaled_by_operator.get_entry(ii, jj));
      }
    }
    scaled = matrix_zeros_sparse;
    scaled_by_operator = matrix_zeros_sparse;
    scaled.scal(ScalarType(1));
    scaled_by_operator *= ScalarType(1);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), scaled.get_entry(ii, jj));
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), scaled_by_operator.get_entry(ii, jj));
      }
    }
    scaled = matrix_ones;
    scaled_by_operator = matrix_ones;
    scaled.scal(ScalarType(0.5));
    scaled_by_operator *= ScalarType(0.5);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0.5), scaled.get_entry(ii, jj));
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0.5), scaled_by_operator.get_entry(ii, jj));
      }
    }
    scaled = testmatrix_sparse;
    scaled_by_operator = testmatrix_sparse;
    scaled.scal(ScalarType(-1.25));
    scaled_by_operator *= ScalarType(-1.25);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(
            Common::FloatCmp::eq(testmatrix_sparse.get_entry(ii, jj) * ScalarType(-1.25), scaled.get_entry(ii, jj)));
        EXPECT_TRUE(Common::FloatCmp::eq(testmatrix_sparse.get_entry(ii, jj) * ScalarType(-1.25),
                                         scaled_by_operator.get_entry(ii, jj)));
      }
    }
    scaled = testmatrix_1;
    scaled_by_operator = testmatrix_1;
    scaled.scal(ScalarType(10));
    scaled_by_operator *= ScalarType(10);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(Common::FloatCmp::eq(testmatrix_1.get_entry(ii, jj) * ScalarType(10), scaled.get_entry(ii, jj)));
        EXPECT_TRUE(Common::FloatCmp::eq(testmatrix_1.get_entry(ii, jj) * ScalarType(10),
                                         scaled_by_operator.get_entry(ii, jj)));
      }
    }
    MatrixImp b = matrix_ones;
    b.scal(ScalarType(0));
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(Common::FloatCmp::eq(ScalarType(1), matrix_ones.get_entry(ii, jj))) << "check copy-on-write";
      }
    }

    // test multiply
    const auto A = testmatrix_sparse;
    const auto B = matrix_ones;
    const auto A_squared = A*A;
    const auto B_squared = B*B;
    const auto A_times_B = A*B;
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        ScalarType A_squared_ii_jj(0.);
        ScalarType B_squared_ii_jj(0.);
        ScalarType A_times_B_ii_jj(0.);
        for (size_t kk = 0; kk < cols; ++kk) {
          A_squared_ii_jj += A.get_entry(ii, kk) * A.get_entry(kk, jj);
          B_squared_ii_jj += B.get_entry(ii, kk) * B.get_entry(kk, jj);
          A_times_B_ii_jj += A.get_entry(ii, kk) * B.get_entry(kk, jj);
        }
        DXTC_EXPECT_FLOAT_EQ(A_squared_ii_jj, A_squared.get_entry(ii, jj));
        DXTC_EXPECT_FLOAT_EQ(B_squared_ii_jj, B_squared.get_entry(ii, jj));
        DXTC_EXPECT_FLOAT_EQ(A_times_B_ii_jj, A_times_B.get_entry(ii, jj));
      } // jj
    } // ii

    // test axpy
    MatrixImp result_axpy = matrix_zeros_dense;
    result_axpy.axpy(ScalarType(1.5), matrix_ones);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(1.5), result_axpy.get_entry(ii, jj));
      }
    }
    result_axpy = matrix_zeros_sparse;
    result_axpy.axpy(ScalarType(-1.5), matrix_zeros_sparse);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_DOUBLE_OR_COMPLEX_EQ(RealType(0), result_axpy.get_entry(ii, jj));
      }
    }
    result_axpy = testmatrix_sparse;
    result_axpy.axpy(ScalarType(-0.5), testmatrix_sparse);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(
            Common::FloatCmp::eq(ScalarType(0.5) * testmatrix_sparse.get_entry(ii, jj), result_axpy.get_entry(ii, jj)));
      }
    }
    result_axpy = testmatrix_1;
    result_axpy.axpy(ScalarType(2), testmatrix_2);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(
            Common::FloatCmp::eq(ScalarType(2) * testmatrix_2.get_entry(ii, jj) + testmatrix_1.get_entry(ii, jj),
                                 result_axpy.get_entry(ii, jj)));
      }
    }
    b = matrix_zeros_dense;
    b.axpy(ScalarType(1), matrix_ones);
    for (size_t ii = 0; ii < rows; ++ii) {
      for (size_t jj = 0; jj < cols; ++jj) {
        EXPECT_TRUE(Common::FloatCmp::eq(ScalarType(0), matrix_zeros_dense.get_entry(ii, jj))) << "check copy-on-write";
      }
    }
    VectorImp res_mtv = vector_zeros, res_mv = vector_zeros;
    matrix_ones.mtv(vector_ones, res_mtv);
    matrix_ones.mtv(vector_ones, res_mv);
    for (size_t ii = 0; ii < rows; ++ii) {
        DXTC_EXPECT_FLOAT_EQ(res_mtv[ii], res_mv[ii]);
    }
    // test clear_row/col, unit_row/col
    auto testm1 = matrix_ones;
    testm1 *= 2.;
    testm1.clear_row(0);
    testm1.clear_row(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        DXTC_EXPECT_FLOAT_EQ(testm1.get_entry(ii, jj), ii == 0 || ii == 2 ? ScalarType(0.) : ScalarType(2.));
    }
    testm1 = matrix_ones;
    testm1 *= 2.;
    testm1.clear_col(0);
    testm1.clear_col(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        DXTC_EXPECT_FLOAT_EQ(testm1.get_entry(ii, jj), jj == 0 || jj == 2 ? ScalarType(0.) : ScalarType(2.));
    }
    testm1 = matrix_ones;
    testm1 *= 2.;
    testm1.unit_row(0);
    testm1.unit_row(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        DXTC_EXPECT_FLOAT_EQ(testm1.get_entry(ii, jj), ii == 0 || ii == 2 ? ScalarType(ii == jj) : 2.);
    }
    testm1 = matrix_ones;
    testm1 *= 2.;
    testm1.unit_col(0);
    testm1.unit_col(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (size_t jj = 0; jj < dim; ++jj)
        DXTC_EXPECT_FLOAT_EQ(testm1.get_entry(ii, jj), jj == 0 || jj == 2 ? ScalarType(ii == jj) : 2.);
    }
    auto testm2 = testmatrix_sparse2;
    testm2.clear_row(0);
    testm2.clear_row(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (auto&& jj : sparse_pattern2.inner(ii))
        DXTC_EXPECT_FLOAT_EQ(testm2.get_entry(ii, jj), ii == 0 || ii == 2 ? ScalarType(0.) : ScalarType(2.));
    }
    testm2 = testmatrix_sparse2;
    testm2.clear_col(0);
    testm2.clear_col(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (auto&& jj : sparse_pattern2.inner(ii))
        DXTC_EXPECT_FLOAT_EQ(testm2.get_entry(ii, jj), jj == 0 || jj == 2 ? ScalarType(0.) : ScalarType(2.));
    }
    testm2 = testmatrix_sparse2;
    testm2.unit_row(0);
    testm2.unit_row(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (auto&& jj : sparse_pattern2.inner(ii))
        DXTC_EXPECT_FLOAT_EQ(testm2.get_entry(ii, jj), ii == 0 || ii == 2 ? ScalarType(ii == jj) : 2.);
    }
    testm2 = testmatrix_sparse2;
    testm2.unit_col(0);
    testm2.unit_col(2);
    for (size_t ii = 0; ii < dim; ++ii) {
      for (auto&& jj : sparse_pattern2.inner(ii))
        DXTC_EXPECT_FLOAT_EQ(testm2.get_entry(ii, jj), jj == 0 || jj == 2 ? ScalarType(ii == jj) : 2.);
    }
  } // void produces_correct_results() const
}; // struct MatrixTest

TEST_F(MatrixTest_{{T_NAME}}, fulfills_interface)
{
  this->fulfills_interface();
}
TEST_F(MatrixTest_{{T_NAME}}, produces_correct_results)
{
  this->produces_correct_results();
}

{% endfor %}
