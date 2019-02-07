// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2019)

#ifndef DUNE_GDT_NORMS_HH
#define DUNE_GDT_NORMS_HH

#include <dune/grid/common/gridview.hh>

#include <dune/xt/grid/type_traits.hh>
#include <dune/xt/functions/interfaces/element-functions.hh>

#include <dune/gdt/local/bilinear-forms/integrals.hh>
#include <dune/gdt/local/integrands/elliptic.hh>
#include <dune/gdt/local/integrands/product.hh>
#include <dune/gdt/operators/localizable-bilinear-form.hh>


namespace Dune {
namespace GDT {


template <class GridViewType, size_t s_r, size_t s_rC, class SF, size_t r_r, size_t r_rC, class RF, class F = double>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value,
                 LocalizableBilinearFormBase<GridViewType, s_r, s_rC, SF, F, r_r, r_rC, RF>>
make_localizable_l2_product(
    const GridViewType& grid_view,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, s_r, s_rC, SF>& left,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r_r, r_rC, RF>& right)
{
  using E = XT::Grid::extract_entity_t<GridViewType>;
  auto localizable_product = make_localizable_bilinear_form(grid_view, left, right);
  localizable_product.append(LocalElementIntegralBilinearForm<E, r_r, r_rC, RF, F, s_r, s_rC, SF>(
      LocalElementProductIntegrand<E, r_r, r_rC, RF, F, s_r, s_rC, SF>()));
  return localizable_product;
}


template <class GridViewType, size_t s_r, size_t s_rC, class SF, size_t r_r, size_t r_rC, class RF, class F = double>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value, F>
l2_product(const GridViewType& grid_view,
           const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, s_r, s_rC, SF>& left,
           const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r_r, r_rC, RF>& right)
{
  auto product = make_localizable_l2_product(grid_view, left, right);
  product.assemble();
  return product.result();
}


template <class GridViewType, size_t r, size_t rC, class R, class F = double>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value, F>
l2_norm(const GridViewType& grid_view,
        const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, rC, R>& func)
{
  return std::sqrt(l2_product(grid_view, func, func));
}


template <class GridViewType, class F, size_t r>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value, LocalizableBilinearFormBase<GridViewType, r, 1, F, F, 1, 1, F>>
make_localizable_elliptic_product(
    const GridViewType& grid_view,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, 1, 1, F>& diffusion_factor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>,
                                               GridViewType::dimension,
                                               GridViewType::dimension,
                                               F>& diffusion_tensor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, 1, F>& left,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, 1, F>& right)
{
  using E = XT::Grid::extract_entity_t<GridViewType>;
  auto localizable_product = make_localizable_bilinear_form(grid_view, left, right);
  localizable_product.append(LocalElementIntegralBilinearForm<E, r, 1, F, F, r, 1, F>(
      LocalEllipticIntegrand<E, r, F>(diffusion_factor, diffusion_tensor)));
  return localizable_product;
}


template <class GridViewType, class F, size_t r>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value, F> elliptic_product(
    const GridViewType& grid_view,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, 1, 1, F>& diffusion_factor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>,
                                               GridViewType::dimension,
                                               GridViewType::dimension,
                                               F>& diffusion_tensor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, 1, F>& left,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, 1, F>& right)
{
  auto product = make_localizable_elliptic_product(grid_view, diffusion_factor, diffusion_tensor, left, right);
  product.assemble();
  return product.result();
}


template <class GridViewType, class F, size_t r>
std::enable_if_t<XT::Grid::is_view<GridViewType>::value, F> elliptic_norm(
    const GridViewType& grid_view,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, 1, 1, F>& diffusion_factor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>,
                                               GridViewType::dimension,
                                               GridViewType::dimension,
                                               F>& diffusion_tensor,
    const XT::Functions::GridFunctionInterface<XT::Grid::extract_entity_t<GridViewType>, r, 1, F>& func)
{
  return std::sqrt(elliptic_product(grid_view, diffusion_factor, diffusion_tensor, func, func));
}


} // namespace GDT
} // namespace Dune

#endif // DUNE_GDT_NORMS_HH
