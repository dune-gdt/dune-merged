// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017 - 2018)
//   René Fritze     (2018 - 2019)
//   Tobias Leibner  (2018, 2020)

#ifndef DUNE_XT_LA_EIGEN_CONTAINER_CONVERSION_HH
#define DUNE_XT_LA_EIGEN_CONTAINER_CONVERSION_HH

#include <dune/xt/common/matrix.hh>
#include <dune/xt/common/numeric_cast.hh>
#include <dune/xt/common/vector.hh>
#include <dune/xt/common/type_traits.hh>

#include <dune/xt/la/container/matrix-interface.hh>
#include <dune/xt/la/type_traits.hh>

namespace Dune::XT::LA {


// "pull in" the vector variant from Common, cannot be done with using (would pull the matrix one, too)
template <class RangeType, class SourceType>
typename std::enable_if<Common::is_vector<SourceType>::value && Common::is_vector<RangeType>::value, RangeType>::type
convert_to(const SourceType& source)
{
  return Common::convert_to<RangeType>(source);
}


/**
 Matrix variants

 |     Source     |     Range      | convert_to |
 |----------------|----------------|------------|
 | Common, not LA | Common or LA   | 1st        |
 | LA, not Common | Common, not LA | 2nd        |
 | LA, not Common | LA, not Common | 3rd        |
 */


//! 1st convert_to
template <class RangeType, class SourceType>
typename std::enable_if<Common::is_matrix<SourceType>::value && !is_matrix<SourceType>::value
                            && Common::is_matrix<RangeType>::value,
                        RangeType>::type
convert_to(const SourceType& source)
{
  return Common::convert_to<RangeType>(source);
}


//! 2nd convert_to
template <class RangeType, class S>
typename std::enable_if<Common::is_matrix<RangeType>::value && !is_matrix<RangeType>::value, RangeType>::type
convert_to(const MatrixInterface<S>& source)
{
  const size_t rows = source.rows();
  const size_t cols = source.cols();
  auto ret = Common::create<RangeType>(rows, cols, typename MatrixInterface<S>::ScalarType(0));
  if (source.sparse) {
    const auto pattern = source.pattern();
    for (size_t ii = 0; ii < rows; ++ii)
      for (const auto& jj : pattern.inner(ii))
        Common::set_matrix_entry(ret,
                                 ii,
                                 jj,
#ifndef DXT_DISABLE_CHECKS
                                 Common::numeric_cast<typename Common::MatrixAbstraction<RangeType>::S>(
#endif
                                     source.get_entry(ii, jj)
#ifndef DXT_DISABLE_CHECKS
                                         )
#endif
        );
  } else {
    for (size_t ii = 0; ii < rows; ++ii)
      for (size_t jj = 0; jj < cols; ++jj)
        Common::set_matrix_entry(ret,
                                 ii,
                                 jj,
#ifndef DXT_DISABLE_CHECKS
                                 Common::numeric_cast<typename Common::MatrixAbstraction<RangeType>::S>(
#endif
                                     source.get_entry(ii, jj)
#ifndef DXT_DISABLE_CHECKS
                                         )
#endif
        );
  }
  return ret;
} // ... convert_to(...)


//! 3rd convert_to
template <class RangeType, class S>
typename std::enable_if<is_matrix<RangeType>::value, RangeType>::type convert_to(const MatrixInterface<S>& source)
{
  const size_t rows = source.rows();
  const size_t cols = source.cols();
  if (RangeType::sparse || source.sparse) {
    const auto pattern = source.pattern();
    RangeType ret(rows, cols, pattern);
    for (size_t ii = 0; ii < rows; ++ii)
      for (const auto& jj : pattern.inner(ii))
        ret.set_entry(ii,
                      jj,
#ifndef DXT_DISABLE_CHECKS
                      Common::numeric_cast<typename Common::MatrixAbstraction<RangeType>::S>(
#endif
                          source.get_entry(ii, jj)
#ifndef DXT_DISABLE_CHECKS
                              )
#endif
        );
    return ret;
  }
  RangeType ret(rows, cols);
  for (size_t ii = 0; ii < rows; ++ii)
    for (size_t jj = 0; jj < cols; ++jj)
      ret.set_entry(ii,
                    jj,
#ifndef DXT_DISABLE_CHECKS
                    Common::numeric_cast<typename Common::MatrixAbstraction<RangeType>::S>(
#endif
                        source.get_entry(ii, jj)
#ifndef DXT_DISABLE_CHECKS
                            )
#endif
      );
  return ret;

} // ... convert_to(...)


} // namespace Dune::XT::LA

#endif // DUNE_XT_LA_EIGEN_CONTAINER_CONVERSION_HH
