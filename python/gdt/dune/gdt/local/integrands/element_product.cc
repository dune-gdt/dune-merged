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

#include <dune/gdt/local/integrands/product.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>


namespace Dune {
namespace GDT {
namespace bindings {


template <class G, class E, size_t r = 1, class F = double>
class LocalElementProductIntegrand
{
public:
  using type = GDT::LocalElementProductIntegrand<E, r, F>;
  using base_type = GDT::LocalBinaryElementIntegrandInterface<E, r, 1, F, F, r, 1, F>;
  using bound_type = pybind11::class_<type, base_type>;

public:
  static bound_type bind(pybind11::module& m,
                         const std::string& layer_id = "",
                         const std::string& grid_id = XT::Grid::bindings::grid_name<G>::value(),
                         const std::string& class_id = "local_element_product_integrand")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    std::string class_name = class_id;
    class_name += "_" + grid_id;
    if (!layer_id.empty())
      class_name += "_" + layer_id;
    class_name += "_" + XT::Common::to_string(r) + "d_bases";
    class_name += "_to_scalar";
    if (!std::is_same<F, double>::value)
      class_name += "_" + XT::Common::Typename<F>::value(/*fail_wo_typeid=*/true);
    const auto ClassName = XT::Common::to_camel_case(class_name);
    bound_type c(m, ClassName.c_str(), ClassName.c_str());
    c.def(py::init([](const XT::Functions::GridFunctionInterface<E, r, r, F>& weight,
                      const std::string logging_prefix) { return new type(weight, logging_prefix); }),
          "weight"_a,
          "logging_prefix"_a = "");

    // factory
    const auto FactoryName = XT::Common::to_camel_case(class_id);
    m.def(
        FactoryName.c_str(),
        [](const XT::Functions::GridFunctionInterface<E, r, r, F>& weight, const std::string& logging_prefix) {
          return new type(weight, logging_prefix);
        },
        "weight"_a,
        "logging_prefix"_a = "");

    return c;
  } // ... bind(...)
}; // class LocalElementProductIntegrand


} // namespace bindings
} // namespace GDT
} // namespace Dune


template <class GridTypes = Dune::XT::Grid::bindings::AvailableGridTypes>
struct LocalElementProductIntegrand_for_all_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GV = typename G::LeafGridView;
  using E = Dune::XT::Grid::extract_entity_t<GV>;
  static const constexpr size_t d = G::dimension;

  static void bind(pybind11::module& m)
  {
    using Dune::GDT::bindings::LocalElementProductIntegrand;

    LocalElementProductIntegrand<G, E>::bind(m);
    if (d > 1)
      LocalElementProductIntegrand<G, E, d>::bind(m);
    // add your extra dimensions here
    // ...
    LocalElementProductIntegrand_for_all_grids<Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <>
struct LocalElementProductIntegrand_for_all_grids<Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};


PYBIND11_MODULE(_local_integrands_element_product, m)
{
  namespace py = pybind11;
  using namespace Dune;
  using namespace Dune::XT;
  using namespace Dune::GDT;

  py::module::import("dune.xt.common");
  py::module::import("dune.xt.la");
  py::module::import("dune.xt.grid");
  py::module::import("dune.xt.functions");

  py::module::import("dune.gdt._local_integrands_binary_element_interface");

  LocalElementProductIntegrand_for_all_grids<XT::Grid::bindings::AvailableGridTypes>::bind(m);
}
