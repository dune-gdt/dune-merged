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
#include <dune/xt/grid/dd/glued.hh>
#include <dune/xt/grid/view/coupling.hh>

#include <dune/gdt/local/integrands/ipdg.hh>

#include <python/xt/dune/xt/common/configuration.hh>
#include <python/xt/dune/xt/common/fvector.hh>
#include <python/xt/dune/xt/grid/grids.bindings.hh>
#include <python/xt/dune/xt/grid/traits.hh>


namespace Dune {
namespace GDT {
namespace bindings {


template <class G, class I, class intersection_type>
class LocalIPDGInnerPenaltyIntegrand
{
  using E = XT::Grid::extract_entity_t<G>;
  using GP = XT::Grid::GridProvider<G>;
  static const size_t d = G::dimension;

public:
  using type = GDT::LocalIPDGIntegrands::InnerPenalty<I>;
  using base_type = GDT::LocalQuaternaryIntersectionIntegrandInterface<I>;
  using bound_type = pybind11::class_<type, base_type>;
  using F = typename type::F;

  static bound_type bind(pybind11::module& m,
                         const std::string& layer_id = "",
                         const std::string& grid_id = XT::Grid::bindings::grid_name<G>::value(),
                         const std::string& class_id = "local_IPDG_inner_penalty_integrand")
  {
    namespace py = pybind11;
    using namespace pybind11::literals;

    std::string class_name = class_id;
    class_name += "_" + grid_id;
    if (!layer_id.empty())
      class_name += "_" + layer_id;
    class_name += "_1d_bases";
    class_name += "_to_scalar";
    if (!std::is_same<F, double>::value)
      class_name += "_" + XT::Common::Typename<F>::value(/*fail_wo_typeid=*/true);
    const auto ClassName = XT::Common::to_camel_case(class_name);
    bound_type c(m, ClassName.c_str(), ClassName.c_str());
    c.def(py::init([](const double& penalty,
                      const XT::Functions::GridFunctionInterface<E, d, d, F>& weight,
                      const std::string& logging_prefix,
                      const intersection_type&) {
            return new type(penalty,
                            weight,
                            GDT::LocalIPDGIntegrands::internal::default_intersection_diameter<I>(),
                            logging_prefix);
          }),
          "penalty"_a,
          "weight"_a,
          "logging_prefix"_a = "",
          "intersection_type"_a = XT::Grid::bindings::LeafIntersection());

    // factory
    const auto FactoryName = XT::Common::to_camel_case(class_id);
    m.def(
        FactoryName.c_str(),
        [](const double& penalty,
           const XT::Functions::GridFunctionInterface<E, d, d, F>& weight,
           const std::string& logging_prefix,
           const intersection_type&) {
          return new type(
              penalty, weight, GDT::LocalIPDGIntegrands::internal::default_intersection_diameter<I>(), logging_prefix);
        },
        "penalty"_a,
        "weight"_a,
        "logging_prefix"_a = "",
        "intersection_type"_a = XT::Grid::bindings::LeafIntersection());

    return c;
  } // ... bind(...)
}; // class LocalIPDGInnerPenaltyIntegrand


} // namespace bindings
} // namespace GDT
} // namespace Dune


template <class GridTypes = Dune::XT::Grid::bindings::AvailableGridTypes>
struct LocalIPDGInnerPenaltyIntegrand_for_all_grids
{
  using G = Dune::XT::Common::tuple_head_t<GridTypes>;
  using GV = typename G::LeafGridView;
  using I = Dune::XT::Grid::extract_intersection_t<GV>;
  static const constexpr size_t d = G::dimension;

  static void bind(pybind11::module& m)
  {
    Dune::GDT::bindings::LocalIPDGInnerPenaltyIntegrand<G, I, Dune::XT::Grid::bindings::LeafIntersection>::bind(m,
                                                                                                                "leaf");
#if HAVE_DUNE_GRID_GLUE
    if constexpr (d == 2) {
      using GridGlueType = Dune::XT::Grid::DD::Glued<G, G, Dune::XT::Grid::Layers::leaf>;
      using CI = typename GridGlueType::GlueType::Intersection;
      using CCI = Dune::XT::Grid::internal::CouplingIntersectionWithCorrectNormal<CI, I>;
      using Coupling = Dune::XT::Grid::bindings::CouplingIntersection<G, G>;
      Dune::GDT::bindings::LocalIPDGInnerPenaltyIntegrand<G, CCI, Coupling>::bind(m, "coupling");
    }
#endif
    LocalIPDGInnerPenaltyIntegrand_for_all_grids<Dune::XT::Common::tuple_tail_t<GridTypes>>::bind(m);
  }
};

template <>
struct LocalIPDGInnerPenaltyIntegrand_for_all_grids<Dune::XT::Common::tuple_null_type>
{
  static void bind(pybind11::module& /*m*/) {}
};


PYBIND11_MODULE(_local_integrands_ipdg_inner_penalty, m)
{
  namespace py = pybind11;
  using namespace Dune;
  using namespace Dune::XT;
  using namespace Dune::GDT;

  py::module::import("dune.xt.common");
  py::module::import("dune.xt.la");
  py::module::import("dune.xt.grid");
  py::module::import("dune.xt.functions");
  py::module::import("dune.gdt._local_integrands_quaternary_intersection_interface");

  LocalIPDGInnerPenaltyIntegrand_for_all_grids<XT::Grid::bindings::AvailableGridTypes>::bind(m);
}
