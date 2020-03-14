// This file is part of the dune-xt project:
//   https://github.com/dune-community/dune-xt
// Copyright 2009-2018 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017, 2019)
//   René Fritze     (2017 - 2018)
//   Tobias Leibner  (2018)

#ifndef DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH
#define DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH

#include <sstream>
#include <type_traits>

#include <dune/pybindxi/pybind11.h>
#include <dune/pybindxi/stl.h>

#include <dune/xt/common/configuration.hh>
#include <dune/xt/grid/gridprovider/provider.hh>
#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/grid/layers.hh>
#include <python/dune/xt/common/configuration.hh>
#include "grids.bindings.hh"
#include "layers.bindings.hh"

#include <dune/xt/grid/boundaryinfo.hh>

namespace Dune {
namespace XT {
namespace Grid {
namespace bindings {
namespace internal {


template <class I, class GP, Layers layer, bool anything = true>
class BoundaryInfoFactory
{
public:
  static void bind(pybind11::module& m)
  {
    using namespace pybind11::literals;
    const auto gridname = XT::Grid::bindings::grid_name<typename GP::GridType>::value();

    try { // guard since we might not be the first to do so for this grid/intersection
      m.def("available_boundary_infos",
            [](const GP& /*grid_provider*/) { return XT::Grid::BoundaryInfoFactory<I>::available(); },
            "grid_provider"_a);
      m.def("default_boundary_info_config_on_",
            [](const GP& /*grid_provider*/, const std::string& type) {
              return XT::Grid::BoundaryInfoFactory<I>::default_config(type);
            },
            "grid_provider"_a,
            "type"_a);
      m.def("make_boundary_info_on_",
            [](const GP& /*grid_provider*/, const Common::Configuration& cfg) {
              return XT::Grid::BoundaryInfoFactory<I>::create(cfg).release();
            },
            "grid_provider"_a,
            "cfg"_a);
      std::string postfix{layer_names[layer] + "_layer_" + gridname};
      m.def((std::string("available_boundary_infos_on_") + postfix).c_str(),
            []() { return XT::Grid::BoundaryInfoFactory<I>::available(); });
      m.def((std::string("default_boundary_info_config_on_") + postfix).c_str(),
            [](const std::string& type) { return XT::Grid::BoundaryInfoFactory<I>::default_config(type); },
            "type"_a);
      m.def((std::string("make_boundary_info_on_") + postfix).c_str(),
            [](const Common::Configuration& cfg) { return XT::Grid::BoundaryInfoFactory<I>::create(cfg).release(); },
            "cfg"_a);
    } catch (std::runtime_error&) {
    }
  } // ... bind(...)
}; // class BoundaryInfoFactory


} // namespace internal


template <class Imp, class G, Layers layer>
class BoundaryInfo
{
  typedef typename Imp::IntersectionType I;
  typedef XT::Grid::BoundaryInfo<I> InterfaceType;

public:
  typedef Imp type;
  typedef pybind11::class_<type, InterfaceType> bound_type;

  static void bind(pybind11::module& m, const std::string& class_name, const std::string& layer_name)
  {
    namespace py = pybind11;

    const auto grid_name = bindings::grid_name<G>::value();
    const auto InterfaceName = Common::to_camel_case("BoundaryInfo_" + layer_name + "_" + grid_name);

    // bind interface, guard since we might not be the first to do so for this intersection
    try {
      py::class_<InterfaceType>(m, InterfaceName.c_str(), InterfaceName.c_str());
    } catch (std::runtime_error&) {
    }

    // bind factory
    internal::BoundaryInfoFactory<I, GridProvider<G>, layer>::bind(m);

    // bind class, guard since we might not be the first to do so for this intersection
    try {
      const auto ClassName = Common::to_camel_case(class_name + "_" + layer_name + "_" + grid_name);
      bound_type c(m, ClassName.c_str(), ClassName.c_str());
      c.def_property_readonly_static("static_id", [](const type& /*self*/) { return type::static_id(); });
      c.def("__repr__", [ClassName](const type& /*self*/) { return ClassName; });
    } catch (std::runtime_error&) {
    }
  } // ... bind(...)
}; // class BoundaryInfo


} // namespace bindings
} // namespace Grid
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_GRID_BOUNDARYINFO_BINDINGS_HH
