// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)

#include "config.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/grid/grids.hh>
#include <dune/xt/grid/gridprovider/provider.hh>
#include <dune/xt/la/type_traits.hh>

#include <dune/gdt/interpolations/boundary.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/common/parameter.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>
#include <python/xt/dune/xt/grid/walker.hh>
#include <python/xt/dune/xt/la/traits.hh>
#include <python/gdt/dune/gdt/operators/interfaces.hh>


namespace Dune {
namespace GDT {
namespace bindings {


template <class V, class VectorTag, class GV, size_t r = 1>
class boundary_interpolation
{
  using G = std::decay_t<XT::Grid::extract_grid_t<GV>>;
  static const size_t d = G::dimension;
  using GP = XT::Grid::GridProvider<G>;
  using E = XT::Grid::extract_entity_t<GV>;
  using I = XT::Grid::extract_intersection_t<GV>;
  using GF = XT::Functions::GridFunction<E, r>;
  using DF = DiscreteFunction<V, GV, r>;
  using BI = XT::Grid::BoundaryInfo<I>;
  using S = typename DF::SpaceType;

public:
  static void bind(pybind11::module& m, const std::string& function_id = "boundary_interpolation")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    m.def(
        function_id.c_str(),
        [](GF source, DF& target, const BI& boundary_info, const XT::Grid::BoundaryType& target_boundary_type) {
          GDT::boundary_interpolation(source, target, boundary_info, target_boundary_type);
        },
        "source"_a,
        "target"_a,
        "boundary_info"_a,
        "target_boundary_type"_a,
        py::call_guard<py::gil_scoped_release>());
    if (std::is_same<VectorTag, XT::LA::bindings::Istl>::value)
      m.def(
          function_id.c_str(),
          [](GF source,
             const S& target_space,
             const BI& boundary_info,
             const XT::Grid::BoundaryType& target_boundary_type,
             const VectorTag&) {
            return GDT::boundary_interpolation<V>(source, target_space, boundary_info, target_boundary_type);
          },
          "source"_a,
          "target"_a,
          "boundary_info"_a,
          "target_boundary_type"_a,
          "la_backend"_a = XT::LA::bindings::Istl(),
          py::call_guard<py::gil_scoped_release>());
    else
      m.def(
          function_id.c_str(),
          [](GF source,
             const S& target_space,
             const BI& boundary_info,
             const XT::Grid::BoundaryType& target_boundary_type,
             const VectorTag&) {
            return GDT::boundary_interpolation<V>(source, target_space, boundary_info, target_boundary_type);
          },
          "source"_a,
          "target"_a,
          "boundary_info"_a,
          "target_boundary_type"_a,
          "la_backend"_a,
          py::call_guard<py::gil_scoped_release>());
  } // ... bind(...)
}; // class boundary_interpolation


} // namespace bindings
} // namespace GDT
} // namespace Dune


template <class V, class VT, class GridTypes = Dune::XT::Grid::bindings::AvailableGridTypes>
struct boundary_interpolation_for_all_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GV = typename G::LeafGridView;
  static const constexpr size_t d = G::dimension;

  static void bind(pybind11::module& m)
  {
    using Dune::GDT::bindings::boundary_interpolation;

    boundary_interpolation<V, VT, GV>::bind(m);
    if (d > 1)
      boundary_interpolation<V, VT, GV, d>::bind(m);
    // add your extra dimensions here
    // ...
    boundary_interpolation_for_all_grids<V, VT, Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <class V, class VT>
struct boundary_interpolation_for_all_grids<V, VT, Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};


PYBIND11_MODULE(_interpolations_boundary, m)
{
  namespace py = pybind11;
  using namespace Dune;
  using namespace Dune::XT;
  using namespace Dune::GDT;

  py::module::import("dune.xt.common");
  py::module::import("dune.xt.la");
  py::module::import("dune.xt.grid");
  py::module::import("dune.xt.functions");

  py::module::import("dune.gdt._discretefunction_discretefunction");
  py::module::import("dune.gdt._spaces_interface");

#if 0
  // bindings for all but dune-istl disabled for the moment
  boundary_interpolation_for_all_grids<LA::CommonDenseVector<double>,
                                      LA::bindings::Common,
                                      XT::Grid::bindings::AvailableGridTypes>::bind(m);
#  if HAVE_EIGEN
  boundary_interpolation_for_all_grids<LA::EigenDenseVector<double>, LA::bindings::Eigen, XT::Grid::bindings::AvailableGridTypes>::
      bind(m);
#  endif
#endif // 0
  boundary_interpolation_for_all_grids<LA::IstlDenseVector<double>,
                                       LA::bindings::Istl,
                                       XT::Grid::bindings::AvailableGridTypes>::bind(m);
}
