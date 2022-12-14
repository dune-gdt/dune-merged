// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   René Fritze     (2018 - 2019)
//   Tobias Leibner  (2018, 2020)

#include <dune/xt/test/main.hxx>

#include <dune/xt/common/matrix.hh>
#include <dune/xt/common/fmatrix.hh>

using MatrixTypes =
    ::testing::Types<std::tuple<Dune::FieldMatrix<int, 1, 1>, Int<1>, Int<1>>,
                     std::tuple<Dune::FieldMatrix<int, 2, 3>, Int<2>, Int<3>>,
                     std::tuple<Dune::FieldMatrix<int, 3, 1>, Int<3>, Int<1>>,
                     std::tuple<Dune::XT::Common::FieldMatrix<int, 1, 1>, Int<1>, Int<1>>,
                     std::tuple<Dune::XT::Common::FieldMatrix<int, 2, 3>, Int<2>, Int<3>>,
                     std::tuple<Dune::XT::Common::FieldMatrix<int, 3, 1>, Int<3>, Int<1>>,
                     std::tuple<Dune::XT::Common::BlockedFieldMatrix<int, 1, 1, 1>, Int<1>, Int<1>, Int<1>>,
                     std::tuple<Dune::XT::Common::BlockedFieldMatrix<int, 2, 2, 2>, Int<2>, Int<2>, Int<2>>,
                     std::tuple<Dune::XT::Common::BlockedFieldMatrix<int, 2, 2, 3>, Int<2>, Int<2>, Int<3>>,
                     std::tuple<Dune::XT::Common::BlockedFieldMatrix<int, 2, 3, 2>, Int<2>, Int<3>, Int<2>>,
                     std::tuple<Dune::DynamicMatrix<int>, Int<1>, Int<1>>,
                     std::tuple<Dune::DynamicMatrix<int>, Int<2>, Int<3>>,
                     std::tuple<Dune::DynamicMatrix<int>, Int<3>, Int<1>>>;


template <class Tuple>
struct SerializeTest : public ::testing::Test
{
  using MatrixType = typename std::tuple_element<0, Tuple>::type;
  using M = Dune::XT::Common::MatrixAbstraction<MatrixType>;
  using ScalarType = typename M::ScalarType;
  static constexpr bool is_block_matrix = std::tuple_size<Tuple>::value == 4;
  static constexpr size_t num_blocks = std::tuple_element<1, Tuple>::type::value; // only for BlockedFieldMatrix
  static constexpr size_t rows =
      is_block_matrix ? std::tuple_element<1, Tuple>::type::value * std::tuple_element<2, Tuple>::type::value
                      : std::tuple_element<1, Tuple>::type::value;
  static constexpr size_t cols = is_block_matrix
      ? std::tuple_element<1, Tuple>::type::value * std::tuple_element < is_block_matrix ? 3 : 2,
                          Tuple > ::type::value : std::tuple_element<2, Tuple>::type::value;

  SerializeTest()
    : matrix_(M::create(rows, cols))
  {
    if constexpr (is_block_matrix) {
      constexpr size_t block_rows = rows / num_blocks;
      constexpr size_t block_cols = cols / num_blocks;
      for (size_t jj = 0; jj < num_blocks; ++jj)
        for (size_t ll = 0; ll < block_rows; ++ll)
          for (size_t mm = 0; mm < block_cols; ++mm) {
            const size_t row = jj * block_rows + ll;
            const size_t col = jj * block_cols + mm;
            M::set_entry(matrix_, row, col, static_cast<ScalarType>(100 * row + col));
          }
    } else {
      for (size_t ii = 0; ii < rows; ++ii)
        for (size_t jj = 0; jj < cols; ++jj)
          M::set_entry(matrix_, ii, jj, static_cast<ScalarType>(100 * ii + jj));
    }
  }

  template <class T>
  void check_type()
  {
    auto data_rowwise = Dune::XT::Common::serialize_rowwise<T>(matrix_);
    size_t cc = 0;
    for (size_t ii = 0; ii < rows; ++ii)
      for (size_t jj = 0; jj < cols; ++jj)
        EXPECT_EQ(M::get_entry(matrix_, ii, jj), data_rowwise[cc++]);
    auto data_colwise = Dune::XT::Common::serialize_colwise<T>(matrix_);
    cc = 0;
    for (size_t jj = 0; jj < cols; ++jj)
      for (size_t ii = 0; ii < rows; ++ii)
        EXPECT_EQ(M::get_entry(matrix_, ii, jj), data_colwise[cc++]);
  }

  void check_default()
  {
    auto data_rowwise = Dune::XT::Common::serialize_rowwise(matrix_);
    size_t cc = 0;
    for (size_t ii = 0; ii < rows; ++ii)
      for (size_t jj = 0; jj < cols; ++jj)
        EXPECT_EQ(M::get_entry(matrix_, ii, jj), data_rowwise[cc++]);
    auto data_colwise = Dune::XT::Common::serialize_colwise(matrix_);
    cc = 0;
    for (size_t jj = 0; jj < cols; ++jj)
      for (size_t ii = 0; ii < rows; ++ii)
        EXPECT_EQ(M::get_entry(matrix_, ii, jj), data_colwise[cc++]);
  }

  MatrixType matrix_;
}; // struct SerializeTest


TYPED_TEST_SUITE(SerializeTest, MatrixTypes);
TYPED_TEST(SerializeTest, default)
{
  this->check_default();
}
TYPED_TEST(SerializeTest, int)
{
  this->template check_type<int>();
}
TYPED_TEST(SerializeTest, long_int)
{
  this->template check_type<long int>();
}
TYPED_TEST(SerializeTest, long_long_int)
{
  this->template check_type<long long int>();
}
TYPED_TEST(SerializeTest, ssize_t)
{
  this->template check_type<ssize_t>();
}
TYPED_TEST(SerializeTest, unsigned_int)
{
  this->template check_type<unsigned int>();
}
TYPED_TEST(SerializeTest, unsigned_long_int)
{
  this->template check_type<unsigned long int>();
}
TYPED_TEST(SerializeTest, unsigned_long_long_int)
{
  this->template check_type<unsigned long long int>();
}
TYPED_TEST(SerializeTest, size_t)
{
  this->template check_type<size_t>();
}
TYPED_TEST(SerializeTest, float)
{
  this->template check_type<float>();
}
TYPED_TEST(SerializeTest, double)
{
  this->template check_type<double>();
}
