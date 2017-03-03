// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2017 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)

#ifndef DUNE_GDT_OPERATORS_ELLIPTIC_BINDINGS_HH
#define DUNE_GDT_OPERATORS_ELLIPTIC_BINDINGS_HH
//#if HAVE_DUNE_PYBINDXI

#include <dune/pybindxi/pybind11.h>

#include <dune/xt/grid/grids.bindings.hh>

#include <dune/gdt/spaces.bindings.hh>

#include "base.bindings.hh"
#include "elliptic.hh"

namespace Dune {
namespace GDT {
namespace bindings {


template <class DF,
          typename DT, // may be void
          class R,
          class M = typename XT::LA::Container<typename R::RangeFieldType>::MatrixType /*,
          class GV = typename R::GridViewType,
          class S = R,
          class F = typename R::RangeFieldType*/>
class EllipticMatrixOperator
{
public:
  typedef GDT::EllipticMatrixOperator<DF, DT, R, M /*, GV, S, F*/> type;
  typedef pybind11::class_<type> bound_type;

private:
  template <bool single_diffusion = std::is_same<DT, void>::value, bool anything = false>
  struct diffusion_switch
  {
    static std::string suffix()
    {
      return "diffusion_factor_and_tensor";
    }

    template <class C>
    static void addbind_factory_methods(pybind11::module& m, const std::string& method_id, const std::string& la_id)
    {
      namespace py = pybind11;
      using namespace pybind11::literals;

      m.def(
          std::string(method_id + "__" + la_id).c_str(),
          [](const DF& diffusion_factor, const DT& diffusion_tensor, const R& space, const size_t over_integrate) {
            return make_elliptic_matrix_operator<M>(diffusion_factor, diffusion_tensor, space, over_integrate)
                .release(); //    <- b.c. EllipticMatrixOperator is not movable, returning the raw pointer lets pybind11
          }, //                                                                              correctly manage the memory
          "diffusion_factor"_a,
          "diffusion_tensor"_a,
          "space"_a,
          "over_integrate"_a = 0,
          py::keep_alive<0, 1>(),
          py::keep_alive<0, 2>(),
          py::keep_alive<0, 3>());

      m.def(
          std::string(method_id).c_str(),
          [](const DF& diffusion_factor,
             const DT& diffusion_tensor,
             M& matrix,
             const R& space,
             const size_t over_integrate) {
            return make_elliptic_matrix_operator(diffusion_factor, diffusion_tensor, matrix, space, over_integrate)
                .release(); //                                                                     <- s.a. for release()
          },
          "diffusion_factor"_a,
          "diffusion_tensor"_a,
          "matrix"_a,
          "space"_a,
          "over_integrate"_a = 0,
          py::keep_alive<0, 1>(),
          py::keep_alive<0, 2>(),
          py::keep_alive<0, 3>());
    } // ... addbind_factory_methods(...)
  }; // struct diffusion_switch

  template <bool anything>
  struct diffusion_switch<true, anything>
  {
    static std::string suffix()
    {
      return "single_diffusion";
    }

    template <class C>
    static void addbind_factory_methods(pybind11::module& m, const std::string& method_id, const std::string& la_id)
    {
      namespace py = pybind11;
      using namespace pybind11::literals;

      m.def(
          std::string(method_id + "__" + la_id).c_str(),
          [](const DF& diffusion, const R& space, const size_t over_integrate) {
            return make_elliptic_matrix_operator<M>(diffusion, space, over_integrate).release(); // <- s.a. for
          }, //                                                                                               .release()
          "diffusion"_a,
          "space"_a,
          "over_integrate"_a = 0,
          py::keep_alive<0, 1>(),
          py::keep_alive<0, 2>());

      m.def(
          std::string(method_id).c_str(),
          [](const DF& diffusion, M& matrix, const R& space, const size_t over_integrate) {
            return make_elliptic_matrix_operator(diffusion, matrix, space, over_integrate).release(); //     <- s.a. for
          }, //                                                                                               .release()
          "diffusion"_a,
          "matrix"_a,
          "space"_a,
          "over_integrate"_a = 0,
          py::keep_alive<0, 1>(),
          py::keep_alive<0, 2>());
    } // ... addbind_factory_methods(...)
  }; // struct diffusion_switch<..., void>

public:
  static bound_type bind(pybind11::module& m, const std::string& space_id, const std::string& la_id)
  {

    namespace py = pybind11;
    using namespace pybind11::literals;

    const std::string suffix = la_id + "__" + space_id + diffusion_switch<>::suffix();

    auto c = MatrixOperatorBase<type>::bind(m, "EllipticMatrixOperator__" + suffix);

    diffusion_switch<>::template addbind_factory_methods<type>(m, "make_elliptic_matrix_operator", la_id);

    return c;
  } // ... bind(...)
}; // class EllipticMatrixOperator


#define DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(_prefix, _GRID, _LA)                                                      \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, FV_SPACE(_GRID, leaf, gdt, 1, 1), _LA);                                   \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, FV_SPACE(_GRID, level, gdt, 1, 1), _LA)

#define DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(_prefix, _GRID, _LA)                                                      \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, CG_SPACE(_GRID, leaf, fem, 1, 1, 1), _LA);                                \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, CG_SPACE(_GRID, level, fem, 1, 1, 1), _LA);                               \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, DG_SPACE(_GRID, leaf, fem, 1, 1, 1), _LA);                                \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, DG_SPACE(_GRID, level, fem, 1, 1, 1), _LA);                               \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, DG_SPACE(_GRID, leaf, fem, 2, 1, 1), _LA);                                \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, DG_SPACE(_GRID, level, fem, 2, 1, 1), _LA)

#define DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(_prefix, _GRID, _LA)                                                   \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, CG_SPACE(_GRID, leaf, pdelab, 1, 1, 1), _LA);                             \
  DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, CG_SPACE(_GRID, level, pdelab, 1, 1, 1), _LA)

#define DUNE_GDT_OPERATORS_ELLIPTIC_BIND_(_prefix, _SPACE, _LA)                                                        \
  _prefix class EllipticMatrixOperator<XT::Functions::LocalizableFunctionInterface<typename _SPACE::EntityType,        \
                                                                                   typename _SPACE::DomainFieldType,   \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   typename _SPACE::RangeFieldType,    \
                                                                                   1,                                  \
                                                                                   1>,                                 \
                                       XT::Functions::LocalizableFunctionInterface<typename _SPACE::EntityType,        \
                                                                                   typename _SPACE::DomainFieldType,   \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   typename _SPACE::RangeFieldType,    \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   _SPACE::dimDomain>,                 \
                                       _SPACE,                                                                         \
                                       typename XT::LA::Container<typename _SPACE::RangeFieldType,                     \
                                                                  XT::LA::Backends::_LA>::MatrixType>;                 \
  _prefix class EllipticMatrixOperator<XT::Functions::LocalizableFunctionInterface<typename _SPACE::EntityType,        \
                                                                                   typename _SPACE::DomainFieldType,   \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   typename _SPACE::RangeFieldType,    \
                                                                                   1,                                  \
                                                                                   1>,                                 \
                                       void,                                                                           \
                                       _SPACE,                                                                         \
                                       typename XT::LA::Container<typename _SPACE::RangeFieldType,                     \
                                                                  XT::LA::Backends::_LA>::MatrixType>;                 \
  _prefix class EllipticMatrixOperator<XT::Functions::LocalizableFunctionInterface<typename _SPACE::EntityType,        \
                                                                                   typename _SPACE::DomainFieldType,   \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   typename _SPACE::RangeFieldType,    \
                                                                                   _SPACE::dimDomain,                  \
                                                                                   _SPACE::dimDomain>,                 \
                                       void,                                                                           \
                                       _SPACE,                                                                         \
                                       typename XT::LA::Container<typename _SPACE::RangeFieldType,                     \
                                                                  XT::LA::Backends::_LA>::MatrixType>


// these lines have to match the corresponding ones in the .cc source file
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, YASP_2D_EQUIDISTANT_OFFSET, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, YASP_2D_EQUIDISTANT_OFFSET, istl_sparse);
#endif
#if HAVE_DUNE_FEM
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, YASP_2D_EQUIDISTANT_OFFSET, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, YASP_2D_EQUIDISTANT_OFFSET, istl_sparse);
#endif
#endif // HAVE_DUNE_FEM
#if HAVE_DUNE_PDELAB
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, YASP_2D_EQUIDISTANT_OFFSET, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, YASP_2D_EQUIDISTANT_OFFSET, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, YASP_2D_EQUIDISTANT_OFFSET, istl_sparse);
#endif
#endif // HAVE_DUNE_PDELAB

#if HAVE_ALUGRID || HAVE_DUNE_ALUGRID
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, ALU_2D_SIMPLEX_CONFORMING, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_GDT(extern template, ALU_2D_SIMPLEX_CONFORMING, istl_sparse);
#endif
#if HAVE_DUNE_FEM
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, ALU_2D_SIMPLEX_CONFORMING, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_FEM(extern template, ALU_2D_SIMPLEX_CONFORMING, istl_sparse);
#endif
#endif // HAVE_DUNE_FEM
#if HAVE_DUNE_PDELAB
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, ALU_2D_SIMPLEX_CONFORMING, common_dense);
#if HAVE_EIGEN
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_dense);
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, ALU_2D_SIMPLEX_CONFORMING, eigen_sparse);
#endif
#if HAVE_DUNE_ISTL
DUNE_GDT_OPERATORS_ELLIPTIC_BIND_PDELAB(extern template, ALU_2D_SIMPLEX_CONFORMING, istl_sparse);
#endif
#endif // HAVE_DUNE_PDELAB
#endif // HAVE_ALUGRID || HAVE_DUNE_ALUGRID


} // namespace bindings
} // namespace GDT
} // namespace Dune

//#endif // HAVE_DUNE_PYBINDXI
#endif // DUNE_GDT_OPERATORS_ELLIPTIC_BINDINGS_HH
