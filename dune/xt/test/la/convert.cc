// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   René Fritze     (2018 - 2019)
//   Tobias Leibner  (2020)

/**
 * This file is intended as a starting point for quick testing.
 */

#define DUNE_XT_COMMON_TEST_MAIN_ENABLE_DEBUG_LOGGING 1
#define DUNE_XT_COMMON_TEST_MAIN_ENABLE_INFO_LOGGING 1
#define DUNE_XT_COMMON_TEST_MAIN_ENABLE_TIMED_LOGGING 1

#include <dune/xt/test/main.hxx> // <- This one has to come first, includes config.h!

#include <dune/xt/la/container.hh>
#include <dune/xt/la/container/conversion.hh>

GTEST_TEST(invert, main)
{
  using K = double;
  constexpr int ROWS = 2, COLS = 2;
#if HAVE_EIGEN
  {
    using ToType = Dune::FieldMatrix<K, ROWS, COLS>;
    using FromType = Dune::XT::LA::EigenDenseMatrix<K>;
    Dune::XT::LA::convert_to<ToType>(FromType{ROWS, COLS});
    Dune::XT::LA::convert_to<ToType>(ToType{});
  }
  {
    using FromType = Dune::FieldMatrix<K, ROWS, COLS>;
    using ToType = Dune::XT::LA::EigenDenseMatrix<K>;
    Dune::XT::LA::convert_to<ToType>(FromType{});
    Dune::XT::LA::convert_to<ToType>(ToType{ROWS, COLS});
  }
  {
    using FromType = Dune::XT::LA::EigenDenseMatrix<K>;
    using ToType = FromType;
    Dune::XT::LA::convert_to<ToType>(FromType{ROWS, COLS});
    Dune::XT::LA::convert_to<ToType>(ToType{ROWS, COLS});
  }
#endif // #if HAVE_EIGEN
  {
    using ToType = Dune::FieldMatrix<K, ROWS, COLS>;
    using FromType = ToType;
    Dune::XT::LA::convert_to<ToType>(FromType{});
    Dune::XT::LA::convert_to<ToType>(ToType{});
  }
}
