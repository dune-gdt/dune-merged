// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   René Fritze     (2018 - 2020)
//   Tobias Leibner  (2020)

#ifndef DUNE_XT_GRID_LAYERS_BINDINGS_HH
#define DUNE_XT_GRID_LAYERS_BINDINGS_HH

#include <dune/xt/grid/layers.hh>

namespace Dune::XT::Grid::bindings {


template <Backends backend>
struct backend_name
{
  static_assert(AlwaysFalse<typename XT::Grid::internal::backend_dependent_typename<backend>::type>::value,
                "Please add a specialization for this backend!");

  static std::string value()
  {
    return "";
  }
};


template <>
struct backend_name<Backends::view>
{
  static std::string value()
  {
    return "view";
  }
};


} // namespace Dune::XT::Grid::bindings

#endif // DUNE_XT_GRID_LAYERS_BINDINGS_HH
