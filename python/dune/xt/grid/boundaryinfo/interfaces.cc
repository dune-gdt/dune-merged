// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)
//   René Fritze     (2020)

#include "config.h"

#include <dune/xt/grid/dd/glued.hh>
#include <dune/xt/grid/grids.hh>
#include <dune/xt/grid/view/coupling.hh>

#include <python/dune/xt/grid/grids.bindings.hh>

#include "interfaces.hh"


template <class GridTypes = Dune::XT::Grid::bindings::AvailableGridTypes>
struct BoundaryInfo_for_all_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GV = typename G::LeafGridView;

  static void bind(pybind11::module& m)
  {
    using Dune::XT::Grid::bindings::grid_name;
    Dune::XT::Grid::bindings::BoundaryInfo<GV>::bind(m, grid_name<G>::value(), "leaf");
    BoundaryInfo_for_all_grids<Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <>
struct BoundaryInfo_for_all_grids<Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};

template <class GridTypes = Dune::XT::Grid::bindings::Available2dGridTypes>
struct BoundaryInfo_for_all_coupling_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GridGlueType = Dune::XT::Grid::DD::Glued<G,G,Dune::XT::Grid::Layers::leaf>;
  using CGV = Dune::XT::Grid::CouplingGridView<GridGlueType>;

  static void bind(pybind11::module& m)
  {
    using Dune::XT::Grid::bindings::grid_name;
    Dune::XT::Grid::bindings::BoundaryInfo<CGV>::bind(m, grid_name<G>::value(), "coupling");
    BoundaryInfo_for_all_coupling_grids<Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <>
struct BoundaryInfo_for_all_coupling_grids<Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};

PYBIND11_MODULE(_grid_boundaryinfo_interfaces, m)
{
  namespace py = pybind11;

  py::module::import("dune.xt.common.timedlogging");

  BoundaryInfo_for_all_grids<>::bind(m);
  BoundaryInfo_for_all_coupling_grids<>::bind(m);
}
