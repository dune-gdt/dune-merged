// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)
//   René Fritze     (2014 - 2016, 2018 - 2019)
//   Tobias Leibner  (2016, 2018, 2020)

#include <dune/xt/test/main.hxx>

#include <array>
#include <thread>
#include <type_traits>
#include <vector>

#include <dune/xt/common/parallel/threadmanager.hh>
#include <dune/xt/common/parallel/threadstorage.hh>
#include <dune/xt/common/parallel/helper.hh>

using namespace Dune::XT;
using namespace Dune::XT::Common;

template <typename ThreadValue>
void value_check(ThreadValue& foo, const typename ThreadValue::ValueType& value)
{
  EXPECT_EQ(*foo, value);
  EXPECT_EQ(int(foo), value);
  EXPECT_EQ(*(foo.operator->()), value);
}

template <typename ThreadValue,
          bool = std::is_const<typename ThreadValue::ValueType>::value || std::is_const<ThreadValue>::value>
struct Checker
{
  static void check_eq(ThreadValue& foo, const typename ThreadValue::ValueType& value)
  {
    auto& const_foo = static_cast<const ThreadValue&>(foo);
    value_check(const_foo, value);
    EXPECT_GT(const_foo.sum(), 0);
  }
};

template <typename ThreadValue>
struct Checker<ThreadValue, false /*valuetype is not const*/>
{
  static void check_eq(ThreadValue& foo, const typename ThreadValue::ValueType& value)
  {
    auto& const_foo = static_cast<const ThreadValue&>(foo);
    value_check(const_foo, value);
    EXPECT_GT(const_foo.sum(), 0);

    const auto new_value = typename ThreadValue::ValueType(9);
    typename ThreadValue::ValueType& bar = *foo;
    bar = new_value;
    value_check(const_foo, new_value);
  }
};

template <typename ThreadValue>
void check_eq(ThreadValue& foo, const typename ThreadValue::ValueType& value)
{
  Checker<ThreadValue>::check_eq(foo, value);
}

using TLSTypes = testing::Types<PerThreadValue<int>, PerThreadValue<const int>>;

template <class T>
struct ThreadValueTest : public testing::Test
{};

TYPED_TEST_SUITE(ThreadValueTest, TLSTypes);
TYPED_TEST(ThreadValueTest, All)
{
  using PTVType = TypeParam;
  {
    typename PTVType::ValueType value(1);
    PTVType foo(value);
    check_eq(foo, value);
    foo = PTVType(1);
    check_eq(foo, value);
    const auto new_value = *foo;
    const PTVType bar(*foo);
    check_eq(bar, new_value);
  }
  {
    typename PTVType::ValueType zero(0);
    PTVType foo(zero);
    size_t num_threads = Dune::XT::Common::threadManager().max_threads();
    std::vector<std::thread> threads(num_threads);
    for (size_t ii = 0; ii < num_threads; ++ii)
      threads[ii] = std::thread([&foo, &zero]() { EXPECT_EQ(*foo, zero); });
    for (size_t ii = 0; ii < num_threads; ++ii)
      threads[ii].join();
    auto sum = foo.accumulate(0, std::plus<typename PTVType::ValueType>());
    EXPECT_EQ(num_threads * zero, sum);
    typename PTVType::ValueType one = 1;
    PTVType bar(one);
    for (size_t ii = 0; ii < num_threads; ++ii)
      threads[ii] = std::thread([&bar, &one]() { EXPECT_EQ(*bar, one); });
    for (size_t ii = 0; ii < num_threads; ++ii)
      threads[ii].join();
    sum = bar.accumulate(0, std::plus<typename PTVType::ValueType>());
    EXPECT_EQ(num_threads * one, sum);
  }
}

GTEST_TEST(ThreadManager, All)
{
  auto& tm = Dune::XT::Common::threadManager();
  EXPECT_LE(tm.current_threads(), tm.max_threads());
  EXPECT_LT(tm.thread(), tm.current_threads());
}
