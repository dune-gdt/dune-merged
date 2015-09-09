// This file is part of the dune-gdt project:
//   http://users.dune-project.org/projects/dune-gdt
// Copyright holders: Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_GDT_TEST_OPERATORS_HH
#define DUNE_GDT_TEST_OPERATORS_HH

#include <dune/common/unused.hh>

#include <dune/stuff/functions/expression.hh>
#include <dune/stuff/functions/constant.hh>
#include <dune/stuff/grid/provider/cube.hh>
#include <dune/stuff/grid/walker.hh>
#include <dune/stuff/la/container.hh>

#include <dune/gdt/operators/interfaces.hh>
#include <dune/gdt/spaces/interface.hh>

namespace Dune {
namespace GDT {
namespace Tests {
namespace internal {


template <class SpaceType>
struct OperatorBaseTraits
{
  static_assert(is_space<SpaceType>::value, "");
  typedef typename SpaceType::GridViewType GridViewType;
  typedef typename GridViewType::template Codim<0>::Entity EntityType;
  typedef typename SpaceType::DomainFieldType DomainFieldType;
  static const size_t dimDomain = SpaceType::dimDomain;
  typedef typename SpaceType::RangeFieldType RangeFieldType;
  static const size_t dimRange       = SpaceType::dimRange;
  static const unsigned int polOrder = SpaceType::polOrder;
  typedef Dune::Stuff::Functions::Expression<EntityType, DomainFieldType, dimDomain, RangeFieldType, 1>
      ScalarFunctionType;
  typedef Dune::Stuff::Functions::Expression<EntityType, DomainFieldType, dimDomain, RangeFieldType, dimRange>
      FunctionType;
  typedef Dune::Stuff::Functions::Constant<EntityType, DomainFieldType, dimDomain, RangeFieldType, dimDomain, dimDomain>
      TensorFunctionType;
  typedef typename Stuff::LA::Container<RangeFieldType, Stuff::LA::default_backend>::VectorType VectorType;
  typedef DiscreteFunction<SpaceType, VectorType> DiscreteFunctionType;
}; // class OperatorBaseTraits


} // namespace internal


template <class SpaceType>
struct OperatorBase
{
  typedef internal::OperatorBaseTraits<SpaceType> Traits;
  typedef typename Traits::GridViewType GridViewType;
  typedef typename GridViewType::Grid GridType;
  typedef Dune::Stuff::Grid::Providers::Cube<GridType> GridProviderType;
  typedef typename Traits::RangeFieldType RangeFieldType;
  typedef typename Traits::ScalarFunctionType ScalarFunctionType;
  typedef typename Traits::FunctionType FunctionType;
  typedef typename Traits::TensorFunctionType TensorFunctionType;
  typedef typename Traits::DiscreteFunctionType DiscreteFunctionType;
  static const size_t dimDomain = Traits::dimDomain;

  OperatorBase()
    : grid_provider_(0.0, 1.0, 3u)
    , space_(Dune::GDT::SpaceTools::GridPartView<SpaceType>::create_leaf(grid_provider_.grid()))
    , scalar_function_("x", "x[0]", 1, "scalar function", {{"1.0", "0.0", "0.0"}})
    , function_("x", {"x[0]", "0", "0"}, 1)
    , tensor_function_(Stuff::Functions::internal::UnitMatrix<RangeFieldType, dimDomain>::value())
    , discrete_function_(space_)
  {
  }

  GridProviderType grid_provider_;
  const SpaceType space_;
  const ScalarFunctionType scalar_function_;
  const FunctionType function_;
  const TensorFunctionType tensor_function_;
  DiscreteFunctionType discrete_function_;
}; // class OperatorBase


template <class SpaceType>
struct LocalizableProductBase : public OperatorBase<SpaceType>
{
  typedef OperatorBase<SpaceType> BaseType;
  using typename BaseType::GridViewType;

  template <class ProductImp>
  void localizable_product_test(ProductImp& prod)
  {
    const auto& DUNE_UNUSED(source) = prod.source();
    const auto& DUNE_UNUSED(range) = prod.range();
    auto& DUNE_UNUSED(non_const_range) = prod.range();

    Stuff::Grid::Walker<GridViewType> walker(this->space_.grid_view());
    walker.add(prod);
    walker.walk();

    auto DUNE_UNUSED(result) = prod.apply2();
  } // ... localizable_product_test(...)
}; // class LocalizableProductBase


} // namespace Tests
} // namespace GDT
} // namespace Dune

#endif // DUNE_GDT_TEST_OPERATORS_HH
