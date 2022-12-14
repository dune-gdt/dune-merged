// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)
//   René Fritze     (2019)
//   Tim Keil        (2018)
//   Tobias Leibner  (2020)

#define DUNE_XT_COMMON_TEST_MAIN_CATCH_EXCEPTIONS 1

#include <dune/xt/test/main.hxx>

#include <dune/xt/grid/grids.hh>
#include <dune/geometry/quadraturerules.hh>
#include <dune/xt/grid/gridprovider/cube.hh>

#include <dune/xt/functions/ESV2007.hh>
#include <dune/xt/functions/grid-function.hh>
#include <dune/xt/functions/expression.hh>
#include <dune/xt/functions/visualization.hh>

using namespace Dune::XT;
using namespace Dune::XT::Functions;

{% for GRIDNAME, GRID in config['types'] %}


struct ESV2007CutoffFunction_from_{{GRIDNAME}} : public ::testing::Test
{
  using GridType = {{GRID}};
  using ElementType = typename GridType::template Codim<0>::Entity;
  static constexpr size_t d = GridType::dimension;
  static constexpr size_t r = d;
  static constexpr size_t rC = d;

  using DiffusionType = ExpressionFunction<d, r, rC>;
  using FunctionType = ESV2007::CutoffFunction<ElementType>;

  using MatrixRangeExpressionType = typename Dune::XT::Common::FieldMatrix<std::string, d, d>;

  using RangeReturnType = typename FunctionType::LocalFunctionType::RangeReturnType;
  using DomainType = typename FunctionType::LocalFunctionType::DomainType;
  using DerivativeRangeReturnType = typename FunctionType::LocalFunctionType::DerivativeRangeReturnType;

  ESV2007CutoffFunction_from_{{GRIDNAME}}()
    : grid_(Dune::XT::Grid::make_cube_grid<GridType>(1, 3., 4))
  {
    grid_.visualize("grid");
  }

  const Grid::GridProvider<GridType> grid_;
};

TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, is_constructible)
{
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  DiffusionType diffusion_function("x", expr, 1);
  FunctionType function(diffusion_function);
}


TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, is_visualizable)
{
  const auto leaf_view = grid_.leaf_view();
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  DiffusionType diffusion_function("x", expr, 1);
  FunctionType function(diffusion_function);
  visualize(function, leaf_view, "test__ESV2007CutoffFunction_from_{{GRIDNAME}}__is_visualizable");
}

TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, is_bindable)
{
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  DiffusionType diffusion_function("x", expr, 1);
  FunctionType function(diffusion_function);
  auto local_f = function.local_function();
  const auto leaf_view = grid_.leaf_view();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
  }
}


TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, local_order)
{
  const int expected_order = 0;
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  DiffusionType diffusion_function("x", expr, 1);
  FunctionType function(diffusion_function);
  auto local_f = function.local_function();
  const auto leaf_view = grid_.leaf_view();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    const auto actual_order = local_f->order();
    EXPECT_EQ(expected_order, actual_order);
  }
}


TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, local_evaluate)
{
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  auto diffusion_function = make_grid_function<ElementType>(DiffusionType("x", expr, 1));
  FunctionType function(diffusion_function);
  auto local_f = function.local_function();
  auto local_diffusion = diffusion_function.local_function();
  double poincare_constant_ = 1.0 / (M_PIl * M_PIl);
  const auto leaf_view = grid_.leaf_view();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    local_diffusion->bind(element);
    double value_ = std::numeric_limits<double>::max();
    double value = std::numeric_limits<double>::max();
    for (const auto& quadrature_point : Dune::QuadratureRules<double, d>::rule(element.type(), local_diffusion->order() + 2)) {
      const auto local_x = quadrature_point.position();
      value = local_f->evaluate(local_x);
      Common::FieldMatrix<double, d, d> diffusion_tensor_value(0.);
      diffusion_tensor_value = local_diffusion->evaluate(local_x);
      value_ = std::min(value_,
                      LA::make_eigen_solver(diffusion_tensor_value,
      {{'{{"type", LA::eigen_solver_types(diffusion_tensor_value).at(0)}, {"assert_positive_eigenvalues", "1e-15"}}'}})
                          .min_eigenvalues(1)
                          .at(0));
      }
    const auto hh = Grid::entity_diameter(element);
    value_ = (poincare_constant_ * hh * hh) / value_;
    EXPECT_EQ(value_, value);
  }
}


TEST_F(ESV2007CutoffFunction_from_{{GRIDNAME}}, local_jacobian)
{
  MatrixRangeExpressionType expr(std::string("0"));
  for (size_t dd = 0; dd < d; ++dd) {
    expr[dd][dd] = "x[" + Common::to_string(dd) + "]";
  }
  DiffusionType diffusion_function("x", expr, 1);
  FunctionType function(diffusion_function);
  auto local_f = function.local_function();
  const auto leaf_view = grid_.leaf_view();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    DerivativeRangeReturnType expected_jacobian = DerivativeRangeReturnType();
    for (const auto& quadrature_point : Dune::QuadratureRules<double, d>::rule(element.type(), 3)) {
      const auto local_x = quadrature_point.position();
      const auto value = local_f->jacobian(local_x);
      EXPECT_EQ(expected_jacobian, value);
    }
  }
}

{% endfor  %}
