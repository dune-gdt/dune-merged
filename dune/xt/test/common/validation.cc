// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012, 2014, 2016 - 2017)
//   René Fritze     (2012 - 2020)
//   Tobias Leibner  (2014, 2016, 2020)

#include <dune/xt/test/main.hxx>

#include <dune/xt/common/validation.hh>
#include <dune/xt/common/type_traits.hh>
#include <dune/xt/common/math.hh>
#include <dune/xt/common/random.hh>
#include <dune/common/tupleutility.hh>
#include <dune/common/exceptions.hh>
#include <dune/common/bigunsignedint.hh>
#include <limits>
#include <iostream>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/array.hpp>

using namespace Dune::XT::Common;
using namespace Dune::XT::Test;

constexpr auto SEED = std::random_device::result_type(0);

struct ValidationTest : public testing::Test
{
  using T = FIELD_TYPE;
  using RNGType = DefaultRNG<T>;
  static constexpr T eps = Epsilon<T>::value;
  using S = VectorAbstraction<T>::S;
  using limit = std::numeric_limits<S>;
  const int eps_fac = 4;
  const S scalar_eps = Epsilon<S>::value;

  void all() const
  {
    using namespace boost::assign;
    {
      const S lower = S(0);
      const S center = S(3);
      const S upper = S(6);

      test(lower, upper, lower + 1);
      test(lower, upper, center);
      test(lower, upper, upper - 1);

      const auto vl = init_bound<T>(lower);
      const auto vu = init_bound<T>(upper);
      RNGType rng(vl, vu, SEED);
      boost::array<T, 10> ar = list_of<T>().repeat_fun(9, rng);
      ValidateInList<T, boost::array<T, 10>> validator(ar);
      for (T t : ar) {
        EXPECT_TRUE(validator(t));
      }
      std::vector<T> a;
      EXPECT_FALSE(ValidateInList<T>(a)(rng()));
    }
  }

  void test(const S lower, const S upper, const S arg) const
  {
    const auto arg_T = init_bound<T>(arg);
    const auto lower_T = init_bound<T>(lower);
    const auto upper_T = init_bound<T>(upper);
    EXPECT_TRUE(ValidateAny<T>()(arg_T));
    EXPECT_TRUE(ValidateLess<T>(arg_T)(upper_T));
    EXPECT_TRUE(ValidateLessOrEqual<T>(arg_T)(arg_T));
    EXPECT_TRUE(ValidateLessOrEqual<T>(arg_T)(upper_T));
    EXPECT_TRUE(ValidateGreaterOrEqual<T>(arg_T)(arg_T));
    EXPECT_TRUE(ValidateGreaterOrEqual<T>(arg_T)(lower_T));
    EXPECT_TRUE(ValidateGreater<T>(arg_T)(lower_T));
    EXPECT_TRUE(ValidateInterval<T>(lower_T, upper_T)(arg_T));
    EXPECT_FALSE(ValidateNone<T>()(arg_T));
  }
};

TEST_F(ValidationTest, All)
{
  this->all();
}
