// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)

#ifndef PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_UNARY_INTERSECTION_COMBINED_HH
#define PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_UNARY_INTERSECTION_COMBINED_HH

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/grid/gridprovider/provider.hh>

#include <dune/gdt/local/integrands/combined.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>

#include "unary_intersection_interface.hh"

namespace Dune {
namespace GDT {
namespace bindings {


template <class G, class I, size_t r = 1, size_t rC = 1, class RF = double, class F = double>
class LocalUnaryIntersectionIntegrandSum : protected LocalUnaryIntersectionIntegrandInterface<G, I, r, rC, RF, F>
{
  using BaseType = LocalUnaryIntersectionIntegrandInterface<G, I, r, rC, RF, F>;

public:
  using type = GDT::LocalUnaryIntersectionIntegrandSum<I, r, rC, RF, F>;
  using bound_type = pybind11::class_<type, typename BaseType::type>;

  static bound_type bind(pybind11::module& m,
                         const std::string& grid_id = XT::Grid::bindings::grid_name<G>::value(),
                         const std::string& layer_id = "",
                         const std::string& class_id = "local_unary_intersection_integrand_sum")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    const auto class_name = class_id + BaseType::id(grid_id, layer_id);
    const auto ClassName = XT::Common::to_camel_case(class_name);
    bound_type c(m, ClassName.c_str(), ClassName.c_str());

    return c;
  } // ... bind(...)
}; // class LocalUnaryIntersectionIntegrandSum


} // namespace bindings
} // namespace GDT
} // namespace Dune

#endif // PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_UNARY_INTERSECTION_COMBINED_HH
