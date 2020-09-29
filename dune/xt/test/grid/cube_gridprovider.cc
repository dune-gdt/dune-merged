// This file is part of the dune-xt project:
//   https://github.com/dune-community/dune-xt
// Copyright 2009-2020 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012 - 2017)
//   René Fritze     (2012 - 2016, 2018 - 2019)
//   Tobias Leibner  (2014 - 2016, 2020)

#include <dune/xt/test/main.hxx>

#include "provider.hh"


struct CubeGridProvider : public GridProviderBase<TESTGRIDTYPE>
{
  using BaseType = GridProviderBase<TESTGRIDTYPE>;

  void check_layers()
  {
    BaseType::check_layers(Dune::XT::Grid::make_cube_grid<TESTGRIDTYPE>());
  }

  void check_visualize()
  {
    BaseType::check_visualize(Dune::XT::Grid::make_cube_grid<TESTGRIDTYPE>());
  }
};


TEST_F(CubeGridProvider, layers)
{
  this->check_layers();
}
TEST_F(CubeGridProvider, visualize)
{
  this->check_visualize();
}
