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
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/grid/gridprovider/provider.hh>
#include <dune/xt/grid/grids.hh>

#include <dune/gdt/local/bilinear-forms/integrals.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/common/parameter.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>
#include <python/xt/dune/xt/grid/traits.hh>


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
class LocalElementIntegralBilinearForm
{
  static const size_t d = G::dimension;

public:
  using type = GDT::LocalElementIntegralBilinearForm<E, t_r, t_rC, TF, F, a_r, a_rC, AF>;
  using base_type = GDT::LocalElementBilinearFormInterface<E, t_r, t_rC, TF, F, a_r, a_rC, AF>;
  using bound_type = pybind11::class_<type, base_type>;

  static bound_type bind(pybind11::module& m,
                         const std::string& layer_id = "",
                         const std::string& grid_id = XT::Grid::bindings::grid_name<G>::value(),
                         const std::string& class_id = "local_element_integral_bilinear_form")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    std::string class_name = class_id;
    class_name += "_" + grid_id;
    if (!layer_id.empty())
      class_name += "_" + layer_id;
    std::string test_string = "";
    test_string += "_" + XT::Common::to_string(t_r) + "d";
    if (t_rC > 1)
      test_string += "x" + XT::Common::to_string(t_rC) + "d";
    if (!std::is_same<TF, double>::value)
      test_string += "_" + XT::Common::Typename<TF>::value(/*fail_wo_typeid=*/true);
    test_string += "_test_basis";
    std::string ansatz_string = "";
    ansatz_string += "_" + XT::Common::to_string(a_r) + "d";
    if (a_rC > 1)
      ansatz_string += "x" + XT::Common::to_string(a_rC) + "d";
    if (!std::is_same<AF, double>::value)
      ansatz_string += "_" + XT::Common::Typename<AF>::value(/*fail_wo_typeid=*/true);
    ansatz_string += "_ansatz_basis";
    class_name += test_string;
    if (!test_string.empty() && !ansatz_string.empty())
      class_name += "_x";
    class_name += ansatz_string;
    class_name += "_to_scalar";
    if (!std::is_same<F, double>::value)
      class_name += "_" + XT::Common::Typename<F>::value(/*fail_wo_typeid=*/true);
    const auto ClassName = XT::Common::to_camel_case(class_name);
    bound_type c(m, ClassName.c_str(), class_id.c_str());
    c.def(py::init<const typename type::IntegrandType&, const int, const std::string&>(),
          "binary_element_integrand"_a,
          "over_integrate"_a = 0,
          "logging_prefix"_a = "");

    // factory
    m.def(
        XT::Common::to_camel_case(class_id).c_str(),
        [](const typename type::IntegrandType& binary_element_integrand,
           const int over_integrate,
           const std::string& logging_prefix) {
          return new type(binary_element_integrand, over_integrate, logging_prefix);
        },
        "binary_element_integrand"_a,
        "over_integrate"_a = 0,
        "logging_prefix"_a = "");

    return c;
  } // ... bind(...)
}; // class LocalElementIntegralBilinearForm


} // namespace bindings
} // namespace GDT
} // namespace Dune


template <class GridTypes = Dune::XT::Grid::bindings::AvailableGridTypes>
struct LocalElementIntegralBilinearForm_for_all_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GV = typename G::LeafGridView;
  using E = Dune::XT::Grid::extract_entity_t<GV>;
  static const constexpr size_t d = G::dimension;
  using F = double;

  static void bind(pybind11::module& m)
  {
    using Dune::GDT::bindings::LocalElementIntegralBilinearForm;

    LocalElementIntegralBilinearForm<G, E>::bind(m);
    if (d > 1) {
      LocalElementIntegralBilinearForm<G, E, 1, 1, F, F, d, 1, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, 1, 1, F, F, d, d, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, 1, F, F, 1, 1, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, 1, F, F, d, 1, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, 1, F, F, d, d, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, d, F, F, 1, 1, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, d, F, F, d, 1, F>::bind(m);
      LocalElementIntegralBilinearForm<G, E, d, d, F, F, d, d, F>::bind(m);
    }
    // add your extra dimensions here
    // ...
    LocalElementIntegralBilinearForm_for_all_grids<Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <>
struct LocalElementIntegralBilinearForm_for_all_grids<Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};


PYBIND11_MODULE(_local_bilinear_forms_element_integrals, m)
{
  namespace py = pybind11;
  using namespace Dune;
  using namespace Dune::XT;
  using namespace Dune::GDT;

  py::module::import("dune.xt.common");
  py::module::import("dune.xt.la");
  py::module::import("dune.xt.grid");
  py::module::import("dune.xt.functions");
  py::module::import("dune.gdt._local_bilinear_forms_element_interface");
  py::module::import("dune.gdt._local_integrands_binary_element_interface");

  LocalElementIntegralBilinearForm_for_all_grids<XT::Grid::bindings::AvailableGridTypes>::bind(m);
}
