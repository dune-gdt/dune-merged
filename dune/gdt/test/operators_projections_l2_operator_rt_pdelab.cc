// This file is part of the dune-gdt project:
//   http://users.dune-project.org/projects/dune-gdt
// Copyright holders: Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#include <dune/stuff/test/main.hxx>

#include "operators/projections/l2.hh"
#include "spaces/rt/pdelab.hh"

using namespace Dune::GDT::Test;

#if HAVE_DUNE_PDELAB


typedef testing::Types<SPACES_RT_PDELAB
#if HAVE_ALUGRID
                       ,
                       SPACES_RT_PDELAB_ALUGRID
#endif
                       > SpaceTypes;

TYPED_TEST_CASE(L2ProjectionOperatorTest, SpaceTypes);
TYPED_TEST(L2ProjectionOperatorTest, constructible_by_ctor)
{
  this->constructible_by_ctor();
}
TYPED_TEST(L2ProjectionOperatorTest, constructible_by_factory)
{
  this->constructible_by_factory();
}
TYPED_TEST(L2ProjectionOperatorTest, free_function_callable)
{
  this->free_function_callable();
}
TYPED_TEST(L2ProjectionOperatorTest, produces_correct_results)
{
  this->produces_correct_results(0.0925927);
}


#else // HAVE_DUNE_PDELAB


TEST(DISABLED_L2ProjectionOperatorTest, constructible_by_ctor)
{
}
TEST(DISABLED_L2ProjectionOperatorTest, constructible_by_factory)
{
}
TEST(DISABLED_L2ProjectionOperatorTest, free_function_callable)
{
}
TEST(DISABLED_L2ProjectionOperatorTest, produces_correct_results)
{
}


#endif // HAVE_DUNE_PDELAB
