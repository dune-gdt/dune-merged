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
//   Tobias Leibner  (2018 - 2020)

#define DUNE_XT_COMMON_TEST_MAIN_CATCH_EXCEPTIONS 1

#include <dune/xt/test/main.hxx>

#include <dune/xt/grid/grids.hh>
#include <dune/geometry/quadraturerules.hh>
#include <dune/xt/grid/gridprovider/cube.hh>

#include <dune/xt/functions/spe10/model2.hh>
#include <dune/xt/functions/visualization.hh>

using namespace Dune::XT;
using namespace Dune::XT::Functions;

{% for GRIDNAME, GRID, r, rC in config['types'] %}

struct Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}} : public ::testing::Test
{
  using GridType = {{GRID}};
  using ElementType = typename GridType::template Codim<0>::Entity;
  static constexpr size_t d = GridType::dimension;
  static constexpr size_t r = {{r}};
  static constexpr size_t rC = {{rC}};

  using FunctionType = Spe10::Model2Function<ElementType, r, rC>;

  using DerivativeRangeType = typename FunctionType::LocalFunctionType::DerivativeRangeType;

  Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}()
    : grid_(Dune::XT::Grid::make_cube_grid<GridType>({0, 0, 0},
                                                     {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                                                      Dune::XT::Functions::Spe10::internal::model_2_length_y,
                                                      Dune::XT::Functions::Spe10::internal::model_2_length_z},
                                                     {6, 22, 8}))
  {
    grid_.visualize("grid");
  }

  const Dune::XT::Grid::GridProvider<GridType> grid_;
};


TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_constructible)
{
#if HAVE_SPE10_DATA
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}

TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, has_default_config)
{
  auto cfg = FunctionType::defaults();
  EXPECT_EQ(cfg.get<std::string>("name"), FunctionType::static_id());
}

TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_creatable)
{
  // auto default_function = FunctionType::create();
  // auto local_f = default_function->local_function();
  // const auto leaf_view = grid_.leaf_view();
  // for (auto&& element : Dune::elements(leaf_view)) {
  //   local_f->bind(element);
  //   const auto actual_order = local_f->order();
  //   EXPECT_EQ(0, actual_order);
  // }
}

TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_visualizable)
{
#if HAVE_SPE10_DATA
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});
  const auto leaf_view = grid_.leaf_view();
  visualize(function, leaf_view, "test__Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}__is_visualizable", /*subsampling=*/false);
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}

TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, is_bindable)
{
#if HAVE_SPE10_DATA
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});

  auto local_f = function.local_function();
  const auto leaf_view = grid_.leaf_view();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
  }
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}

TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, local_order)
{
#if HAVE_SPE10_DATA
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});
  const auto leaf_view = grid_.leaf_view();
  const int expected_order = 0;
  auto local_f = function.local_function();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    const auto actual_order = local_f->order();
    EXPECT_EQ(expected_order, actual_order);
  }
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}


TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, local_evaluate)
{
#if HAVE_SPE10_DATA
  const auto leaf_view = grid_.leaf_view();
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});
  auto local_f = function.local_function();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    for (const auto& quadrature_point : Dune::QuadratureRules<double, d>::rule(element.type(), 3)) {
      const auto local_x = quadrature_point.position();
      [[maybe_unused]] const auto actual_value = local_f->evaluate(local_x);
    }
  }
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}


TEST_F(Spe10Model2Function_from_{{GRIDNAME}}_to_{{r}}_times_{{rC}}, local_jacobian)
{
#if HAVE_SPE10_DATA
  FunctionType function(SPE10_MODEL2_FILENAME,
                        {0, 0, 0},
                        {Dune::XT::Functions::Spe10::internal::model_2_length_x,
                         Dune::XT::Functions::Spe10::internal::model_2_length_y,
                         Dune::XT::Functions::Spe10::internal::model_2_length_z},
                        {6, 22, 8});
  const auto leaf_view = grid_.leaf_view();
  const DerivativeRangeType expected_jacobian = DerivativeRangeType();
  auto local_f = function.local_function();
  for (auto&& element : Dune::elements(leaf_view)) {
    local_f->bind(element);
    for (const auto& quadrature_point : Dune::QuadratureRules<double, d>::rule(element.type(), 3)) {
      const auto local_x = quadrature_point.position();
      const auto actual_jacobian = local_f->jacobian(local_x);
      EXPECT_EQ(expected_jacobian, actual_jacobian);
    }
  }
#else
  std::cout << "Test disabled, missing spe10 data files!" << std::endl;
#endif
}

{% endfor  %}
