// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)

#ifndef PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_BINARY_ELEMENT_COMBINED_HH
#define PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_BINARY_ELEMENT_COMBINED_HH

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/grid/gridprovider/provider.hh>

#include <dune/gdt/local/integrands/combined.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>

#include "binary_element_interface.hh"

namespace Dune {
namespace GDT {
namespace bindings {


template <class G,
          class E,
          size_t t_r = 1,
          size_t t_rC = 1,
          class TF = double,
          class F = double,
          size_t a_r = t_r,
          size_t a_rC = t_rC,
          class AF = TF>
class LocalBinaryElementIntegrandSum
  : protected LocalBinaryElementIntegrandInterface<G, E, t_r, t_rC, TF, F, a_r, a_rC, AF>
{
  using BaseType = LocalBinaryElementIntegrandInterface<G, E, t_r, t_rC, TF, F, a_r, a_rC, AF>;

public:
  using type = GDT::LocalBinaryElementIntegrandSum<E, t_r, t_rC, TF, F, a_r, a_rC, AF>;
  using bound_type = pybind11::class_<type, typename BaseType::type>;

  static bound_type bind(pybind11::module& m,
                         const std::string& grid_id = XT::Grid::bindings::grid_name<G>::value(),
                         const std::string& layer_id = "",
                         const std::string& class_id = "local_binary_element_integrand_sum")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    const auto class_name = class_id + BaseType::id(grid_id, layer_id);
    const auto ClassName = XT::Common::to_camel_case(class_name);
    bound_type c(m, ClassName.c_str(), ClassName.c_str());

    return c;
  } // ... bind(...)
}; // class LocalBinaryElementIntegrandSum


} // namespace bindings
} // namespace GDT
} // namespace Dune

#endif // PYTHON_DUNE_GDT_LOCAL_INTEGRANDS_BINARY_ELEMENT_COMBINED_HH
