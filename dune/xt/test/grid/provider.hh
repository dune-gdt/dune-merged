// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012 - 2017, 2019)
//   René Fritze     (2012 - 2019)
//   Tobias Leibner  (2014 - 2016, 2018, 2020)

#ifndef DUNE_XT_TEST_GRID_PROVIDER_HH
#define DUNE_XT_TEST_GRID_PROVIDER_HH

#include <dune/xt/common/type_traits.hh>

#include <dune/xt/grid/boundaryinfo.hh>
#include <dune/xt/grid/gridprovider.hh>
#include <dune/xt/grid/grids.hh>

#include <dune/xt/test/gtest/gtest.h>

template <class GridType>
struct GridProviderBase : public testing::Test
{
  using GridProviderType = Dune::XT::Grid::GridProvider<GridType>;

  static void check_layers(const GridProviderType& grid_provider)
  {
    using namespace Dune::XT::Grid;
    [[maybe_unused]] const std::shared_ptr<GridType>& grid_ptr = grid_provider.grid_ptr();
    [[maybe_unused]] auto leaf_grid_view_1 = grid_provider.leaf_view();
    [[maybe_unused]] auto leaf_grid_view_2 = grid_provider.template layer<Layers::leaf, Backends::view>();
    for (int level = 0; level <= grid_provider.max_level(); ++level) {
      [[maybe_unused]] auto level_grid_view_1 = grid_provider.level_view(level);
      [[maybe_unused]] auto level_grid_view_2 = grid_provider.template layer<Layers::leaf, Backends::view>(level);
    }
  } // ... check_layers()

  template <class G, bool enabled = true>
  struct Helper
  {
    static void check_visualize(const G& grid_provider)
    {
      const auto type_str = Dune::XT::Grid::bindings::grid_name<GridType>::value();
      grid_provider.visualize();
      grid_provider.visualize(type_str + "_a");
      grid_provider.visualize(Dune::XT::Grid::alldirichlet_boundaryinfo_default_config());
      grid_provider.visualize(Dune::XT::Grid::alldirichlet_boundaryinfo_default_config(), type_str + "_b");
    }
  };

  template <class G>
  struct Helper<G, false>
  {
    static void check_visualize(const G& /*grid_provider*/) {}
  };

  static void check_visualize(const GridProviderType& grid_provider)
  {
    Helper < GridProviderType, GridProviderType::dimDomain<4>::check_visualize(grid_provider);
  }
}; // class ConstGridProviderBase


#endif // DUNE_XT_TEST_GRID_PROVIDER_HH
