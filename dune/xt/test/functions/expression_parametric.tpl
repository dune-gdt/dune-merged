// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)
//   René Fritze     (2019 - 2020)
//   Tim Keil        (2018)
//   Tobias Leibner  (2020)

#define DUNE_XT_COMMON_TEST_MAIN_CATCH_EXCEPTIONS 1

#include <dune/xt/test/main.hxx>

#include <dune/xt/grid/grids.hh>
#include <dune/geometry/quadraturerules.hh>
#include <dune/xt/grid/gridprovider/cube.hh>

#include <dune/xt/functions/expression/parametric.hh>
#include <dune/xt/functions/grid-function.hh>
#include <dune/xt/common/parameter.hh>


using namespace Dune::XT;
using namespace Dune::XT::Functions;

{% for GRIDNAME, GRID, r, rC in config['types'] %}

struct ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}} : public ::testing::Test
{
  using GridType = {{GRID}};
  using ElementType = typename GridType::template Codim<0>::Entity;
  static constexpr size_t d = GridType::dimension;
  static constexpr size_t r = {{r}};
  static constexpr size_t rC = {{rC}};

  using FunctionType = Dune::XT::Functions::ParametricExpressionFunction<d, r, rC>;

  using RangeReturnType = typename FunctionType::RangeReturnType;
  using DomainType = typename FunctionType::DomainType;
  using DerivativeRangeReturnType = typename FunctionType::DerivativeRangeReturnType;
  using RangeExpressionType = typename Dune::XT::Common::FieldVector<std::string, r>;

  ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}()
    : grid_(Dune::XT::Grid::make_cube_grid<GridType>(-1., 1., 4))
  {
  }

  const Dune::XT::Grid::GridProvider<GridType> grid_;
};

TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_constructible)
{
  // construct simple example
  const std::pair<const char*, int> param("t_", 1);
  const Common::ParameterType parameter(param);
  const RangeExpressionType expr(std::string("sin(x[0]t_)"));
  FunctionType function("x", parameter, expr, 3);
}


TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, global_order)
{
  const int expected_order_function = 3;

  const RangeExpressionType expr(std::string("sin(x[0]t_)"));
  FunctionType function("x", {"t_", 1}, expr, 3);

  const auto actual_order_function = function.order();

  EXPECT_EQ(expected_order_function, actual_order_function);
}

TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, global_evaluate)
{
    // constant functions
    for (auto vv : {-10., 3., 17., 41.}) {
      const RangeReturnType expected_value(sin(vv));
      const RangeExpressionType expr(std::string("sin(t_)"));
      FunctionType function("x", {"t_", 1}, expr, 3);
      for (auto point : {-1., -0.5, 0., 0.5, 1.}) {
        const DomainType xx(point);
        const auto actual_value = function.evaluate(xx, {"t_", vv});
        EXPECT_EQ(expected_value, actual_value);
      }
    }

    // non constant function
    for (auto vv : {-10., 3., 17., 41.}) {
      const RangeExpressionType expr(std::string("sin(x[0]t_)"));
      FunctionType function("x", {"t_", 1}, expr, 3);
      for (auto point : {-1., -0.5, 0., 0.5, 1.}) {
        const RangeReturnType expected_value(sin(point * vv));
        const DomainType xx(point);
        const auto actual_value = function.evaluate(xx, {"t_", vv});
        EXPECT_EQ(expected_value, actual_value);
      }
    }
}

TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_bindable)
{
  const auto leaf_view = grid_.leaf_view();
  const RangeExpressionType expr(std::string("sin(x[0]t_)"));
  FunctionType function("x", {"t_", 1}, expr, 3);

  auto localizable_function = make_grid_function<ElementType>(function);
  auto local_f = localizable_function.local_function();

  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
  }
}

TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, local_order)
{
  const int expected_order = 3;

  const RangeExpressionType expr(std::string("sin(x[0]t_)"));
  FunctionType function("x", {"t_", 1}, expr, 3);

  const auto leaf_view = grid_.leaf_view();

  auto localizable_function = make_grid_function<ElementType>(function);
  auto local_f = localizable_function.local_function();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    const auto actual_order = local_f->order();
    EXPECT_EQ(expected_order, actual_order);
  }
}

TEST_F(ParametricExpressionFunction_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, local_evaluate)
{
  const auto leaf_view = grid_.leaf_view();

  const RangeExpressionType expr(std::string("sin(x[0]t_)"));
  FunctionType function("x", {"t_", 1}, expr, 3);
  auto localizable_function = make_grid_function<ElementType>(function);
  auto local_f = localizable_function.local_function();
  for (auto&& element : Dune::elements(leaf_view)) {
    const auto geometry = element.geometry();

    local_f->bind(element);
    for (const auto& quadrature_point : Dune::QuadratureRules<double, d>::rule(element.type(), 3)) {
      const auto local_x = quadrature_point.position();
      const auto global_x = geometry.global(local_x);
      for (auto vv : {-10., 3., 17., 41.}) {
        const RangeReturnType expected_value(sin(global_x[0] * vv));
        const auto actual_value = local_f->evaluate(local_x, {"t_", vv});
        EXPECT_EQ(expected_value, actual_value);
      }
    }
  }
}

{% endfor  %}
