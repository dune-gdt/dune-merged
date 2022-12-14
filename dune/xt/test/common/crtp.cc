// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   René Fritze    (2018 - 2019)
//   Tobias Leibner (2018, 2020)

// This one has to come first (includes the config.h)!
#include <dune/xt/test/main.hxx>

#include <boost/config.hpp>

#include <dune/xt/common/crtp.hh>

template <class Traits>
class TestInterface;

struct FailTraits
{
  using BackendType = double;
  using derived_type = TestInterface<FailTraits>;
};

struct TestImp;

struct ImpTraits
{
  using BackendType = double;
  using derived_type = TestImp;
};

template <class Traits>
class TestInterface : public Dune::XT::Common::CRTPInterface<TestInterface<Traits>, Traits>
{
public:
  using BackendType = typename Traits::BackendType;

#if defined(BOOST_CLANG) && BOOST_CLANG
#  pragma GCC diagnostic push
#  pragma clang diagnostic ignored "-Winfinite-recursion"
#endif
  inline BackendType& backend()
  {
    CHECK_CRTP(this->as_imp().backend());
    return this->as_imp().backend();
  }
#if defined(BOOST_CLANG) && BOOST_CLANG
#  pragma GCC diagnostic pop
#endif

  inline const BackendType& backend() const
  {
    CHECK_CRTP(this->as_imp().backend());
    return this->as_imp().backend();
  }
}; // class ProvidesBackend


struct TestImp : public TestInterface<ImpTraits>
{
  using BackendType = ImpTraits::BackendType;
  BackendType foo_;

  inline BackendType& backend()
  {
    return foo_;
  }

  inline const BackendType& backend() const
  {
    return foo_;
  }
};

// If NDEBUG is defined, the CHECK_CRTP macro does nothing, so the code below will not throw
#ifndef NDEBUG
GTEST_TEST(crtp, fail)
{
  TestInterface<FailTraits> test_iface;
  EXPECT_THROW(
      {
        auto& st = test_iface.backend();
        static_cast<void>(st);
      },
      Dune::XT::Common::Exceptions::CRTP_check_failed);
}
#endif // NDEBUG

GTEST_TEST(crtp, success)
{
  TestImp test_imp;
  auto& st = test_imp.backend();
  static_cast<void>(st);
}
