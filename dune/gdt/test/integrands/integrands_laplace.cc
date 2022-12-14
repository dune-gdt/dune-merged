// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Tobias Leibner (2019)

#include <dune/xt/test/main.hxx> // <- this one has to come first (includes the config.h)!

#include <dune/xt/common/matrix.hh>

#include <dune/xt/la/container/eye-matrix.hh>

#include <dune/xt/functions/generic/function.hh>
#include <dune/xt/functions/generic/grid-function.hh>

#include <dune/gdt/local/integrands/laplace.hh>

#include <dune/gdt/test/integrands/integrands.hh>

namespace Dune {
namespace GDT {
namespace Test {


template <class G>
struct LaplaceIntegrandTest : public IntegrandTest<G>
{
  using BaseType = IntegrandTest<G>;
  using BaseType::d;
  using typename BaseType::D;
  using typename BaseType::DomainType;
  using typename BaseType::E;
  using typename BaseType::GV;
  using typename BaseType::MatrixType;
  using typename BaseType::VectorJacobianType;
  using ScalarIntegrandType = LocalLaplaceIntegrand<E, 1>;
  using VectorIntegrandType = LocalLaplaceIntegrand<E, d>;

  void SetUp() override
  {
    BaseType::SetUp();
    diffusion_tensor_ = std::make_shared<XT::Functions::GenericGridFunction<E, 2, 2>>(
        3,
        [](const E&) {},
        [](const DomainType& x, const XT::Common::Parameter&) {
          VectorJacobianType ret{{x[0], x[1]}, {1., 2.}};
          ret *= x[0] * x[1];
          return ret;
        });
  }

  void is_constructable() override final
  {
    [[maybe_unused]] ScalarIntegrandType scalar_integrand1;
    [[maybe_unused]] ScalarIntegrandType scalar_integrand2(XT::LA::eye_matrix<FieldMatrix<D, d, d>>(d, d));
    const XT::Functions::GenericFunction<d, 2, 2> matrix_function(
        1, [](const DomainType& x, const XT::Common::Parameter&) {
          return VectorJacobianType{{x[0], x[1]}, {1., 2.}};
        });
    [[maybe_unused]] ScalarIntegrandType scalar_integrand3(matrix_function);
    [[maybe_unused]] ScalarIntegrandType scalar_integrand4(*diffusion_tensor_);
    [[maybe_unused]] VectorIntegrandType vector_integrand1;
    [[maybe_unused]] VectorIntegrandType vector_integrand2(XT::LA::eye_matrix<FieldMatrix<D, d, d>>(d, d));
    [[maybe_unused]] VectorIntegrandType vector_integrand3(matrix_function);
    [[maybe_unused]] VectorIntegrandType vector_integrand4(*diffusion_tensor_);
  }

  virtual void evaluates_correctly_for_scalar_bases()
  {
    ScalarIntegrandType scalar_integrand(*diffusion_tensor_);
    const auto element = *(grid_provider_->leaf_view().template begin<0>());
    scalar_integrand.bind(element);
    const auto integrand_order = scalar_integrand.order(*scalar_test_, *scalar_ansatz_);
    DynamicMatrix<D> result(2, 2, 0.);
    for (const auto& quadrature_point : Dune::QuadratureRules<D, d>::rule(element.type(), integrand_order)) {
      const auto& x = quadrature_point.position();
      scalar_integrand.evaluate(*scalar_test_, *scalar_ansatz_, x, result);
      DynamicMatrix<D> expected_result{
          {1, 2 * (x[0] * x[1] + std::pow(x[0], 2))},
          {x[0] * std::pow(x[1], 3) + 3 * x[0] * std::pow(x[1], 2),
           3 * std::pow(x[0], 2) * std::pow(x[1], 4)
               + 6 * (std::pow(x[0], 2) * std::pow(x[1], 3) + std::pow(x[0], 3) * std::pow(x[1], 2))}};
      expected_result *= x[0] * x[1];
      for (size_t ii = 0; ii < 2; ++ii)
        for (size_t jj = 0; jj < 2; ++jj)
          EXPECT_DOUBLE_EQ(expected_result[ii][jj], result[ii][jj]);
    }
  }

  virtual void evaluates_correctly_for_vector_bases()
  {
    VectorIntegrandType integrand(*diffusion_tensor_);
    const auto element = *(grid_provider_->leaf_view().template begin<0>());
    integrand.bind(element);
    const auto integrand_order = integrand.order(*vector_test_, *vector_ansatz_);
    DynamicMatrix<D> result(2, 2, 0.);
    for (const auto& quadrature_point : Dune::QuadratureRules<D, d>::rule(element.type(), integrand_order)) {
      const auto& x = quadrature_point.position();
      integrand.evaluate(*vector_test_, *vector_ansatz_, x, result);
      DynamicMatrix<D> expected_result{
          {0., 0.},
          {x[0] * x[1] + x[0] + x[1] + 2,
           2 * (std::pow(x[0], 2) * x[1] + std::pow(x[1], 2) + std::pow(x[0], 2) + 2 * x[1])}};
      expected_result *= x[0] * x[1];
      for (size_t ii = 1; ii < 2; ++ii)
        for (size_t jj = 0; jj < 2; ++jj) {
          EXPECT_DOUBLE_EQ(expected_result[ii][jj], result[ii][jj]);
        }
    }
  }

  virtual void is_integrated_correctly()
  {
    ScalarIntegrandType integrand(1.);
    const auto& grid_view = grid_provider_->leaf_view();
    // std::string grid_name = XT::Common::Typename<G>::value();
    const auto space = make_continuous_lagrange_space<1>(grid_view, /*polorder=*/2);
    const auto n = space.mapper().size();
    BilinearForm<GV, 1> laplace_form(grid_view);
    laplace_form += LocalElementIntegralBilinearForm<E, 1>(integrand);
    auto laplace_op = laplace_form.template with<MatrixType>(space, space);
    auto walker = XT::Grid::make_walker(grid_view);
    walker.append(laplace_op);
    walker.walk(DXTC_TEST_CONFIG_GET("setup.use_tbb", true));

    const auto mat_data_ptr = XT::Common::serialize_rowwise(laplace_op.matrix());
    const auto min_entry = *std::min_element(mat_data_ptr.begin(), mat_data_ptr.begin() + n * n);
    const auto max_entry = *std::max_element(mat_data_ptr.begin(), mat_data_ptr.begin() + n * n);
    const auto square_sum = std::accumulate(
        mat_data_ptr.begin(), mat_data_ptr.begin() + n * n, 0., [](const auto& a, const auto& b) { return a + b * b; });
    EXPECT_NEAR((is_simplex_grid_ ? -2. : -1.896296296296300), min_entry, 1e-13);
    EXPECT_NEAR((is_simplex_grid_ ? 5.777777777777780 : 6.162962962962970), max_entry, 1e-13);
    EXPECT_NEAR((is_simplex_grid_ ? 2481.524691358029 : 1704.099039780521), square_sum, 5e-12);
    // std::cout << XT::Common::to_string(stiffness_matrix, 15) << std::endl;
  }

  using BaseType::grid_provider_;
  using BaseType::is_simplex_grid_;
  using BaseType::scalar_ansatz_;
  using BaseType::scalar_test_;
  using BaseType::vector_ansatz_;
  using BaseType::vector_test_;
  std::shared_ptr<XT::Functions::GenericGridFunction<E, 2, 2>> diffusion_tensor_;
}; // struct LaplaceIntegrandTest


} // namespace Test
} // namespace GDT
} // namespace Dune


template <class G>
using LaplaceIntegrandTest = Dune::GDT::Test::LaplaceIntegrandTest<G>;
TYPED_TEST_SUITE(LaplaceIntegrandTest, Grids2D);

TYPED_TEST(LaplaceIntegrandTest, is_constructable)
{
  this->is_constructable();
}
TYPED_TEST(LaplaceIntegrandTest, evaluates_correctly_for_scalar_bases)
{
  this->evaluates_correctly_for_scalar_bases();
}

TYPED_TEST(LaplaceIntegrandTest, evaluates_correctly_for_vector_bases)
{
  this->evaluates_correctly_for_vector_bases();
}

TYPED_TEST(LaplaceIntegrandTest, is_integrated_correctly)
{
  this->is_integrated_correctly();
}
