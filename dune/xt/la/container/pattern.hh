// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012 - 2014, 2016 - 2017)
//   René Fritze     (2013 - 2016, 2018 - 2019)
//   Sven Kaulmann   (2014)
//   Tobias Leibner  (2017 - 2020)

#ifndef DUNE_XT_LA_CONTAINER_PATTERN_HH
#define DUNE_XT_LA_CONTAINER_PATTERN_HH

#include <cstddef>
#include <vector>

#include <dune/xt/common/type_traits.hh>

namespace Dune::XT::LA {


class SparsityPatternDefault
{
private:
  using BaseType = std::vector<std::vector<size_t>>;

public:
  using InnerType = BaseType::value_type;
  using OuterIteratorType = typename BaseType::iterator;
  using ConstOuterIteratorType = typename BaseType::const_iterator;

  explicit SparsityPatternDefault(const size_t _size = 0);

  size_t size() const;

  InnerType& inner(const size_t ii);

  const InnerType& inner(const size_t ii) const;

  OuterIteratorType begin();

  ConstOuterIteratorType begin() const;

  OuterIteratorType end();

  ConstOuterIteratorType end() const;

  bool operator==(const SparsityPatternDefault& other) const;

  bool operator!=(const SparsityPatternDefault& other) const;

  SparsityPatternDefault operator+(const SparsityPatternDefault& other) const;

  void insert(const size_t outer_index, const size_t inner_index);

  void sort(const size_t outer_index);

  void sort();

  bool contains(const size_t outer_index, const size_t inner_index) const;

  bool contains(const SparsityPatternDefault& other) const;

  SparsityPatternDefault transposed(const size_t cols) const;

private:
  BaseType vector_of_vectors_;
}; // class SparsityPatternDefault

SparsityPatternDefault dense_pattern(const size_t rows, const size_t cols);

SparsityPatternDefault tridiagonal_pattern(const size_t rows, const size_t cols);

SparsityPatternDefault triangular_pattern(const size_t rows,
                                          const size_t cols,
                                          const Common::MatrixPattern& uplo = Common::MatrixPattern::lower_triangular);

SparsityPatternDefault diagonal_pattern(const size_t rows);
SparsityPatternDefault diagonal_pattern(const size_t rows, const size_t cols, int offset = 0);

SparsityPatternDefault multiplication_pattern(const SparsityPatternDefault& lhs_pattern,
                                              const SparsityPatternDefault& rhs_pattern,
                                              const size_t rhs_cols);


} // namespace Dune::XT::LA

#endif // DUNE_XT_LA_CONTAINER_PATTERN_HH
