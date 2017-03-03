// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2017 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)
//   Rene Milk       (2016)
//   Tobias Leibner  (2016)

#include <dune/xt/common/test/main.hxx> // <- This one has to come first!

#include "projections.hh"
#include "spaces/cg/pdelab.hh"

using namespace Dune::GDT::Test;

#if HAVE_DUNE_PDELAB


typedef testing::Types<SPACES_CG_PDELAB(1)
#if HAVE_DUNE_ALUGRID
                           ,
                       SPACES_CG_PDELAB_ALUGRID(1)
#endif
                       >
    SpaceTypes;

TYPED_TEST_CASE(ProjectionTest, SpaceTypes);
TYPED_TEST(ProjectionTest, produces_correct_results)
{
  this->produces_correct_results();
}


#else // HAVE_DUNE_PDELAB


TEST(DISABLED_ProjectionTest, produces_correct_results)
{
}


#endif // HAVE_DUNE_PDELAB
