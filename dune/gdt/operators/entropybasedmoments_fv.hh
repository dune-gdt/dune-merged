// This file is part of the dune-gdt project:
//   https://github.com/dune-community/dune-gdt
// Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Rene Milk      (2017 - 2018)
//   Tobias Leibner (2017)

#ifndef DUNE_GDT_OPERATORS_FV_ENTROPYBASED_HH
#define DUNE_GDT_OPERATORS_FV_ENTROPYBASED_HH

#include <dune/gdt/operators/interfaces.hh>

namespace Dune {
namespace GDT {


template <class AdvectionOperatorImp, class EntropySolverImp>
class EntropyBasedMomentFvOperator
  : public OperatorInterface<typename AdvectionOperatorImp::MatrixType,
                             typename AdvectionOperatorImp::SGV,
                             AdvectionOperatorImp::s_r>
{
  using BaseType = OperatorInterface<typename AdvectionOperatorImp::MatrixType,
                                     typename AdvectionOperatorImp::SGV,
                                     AdvectionOperatorImp::s_r>;

public:
  using typename BaseType::RangeSpaceType;
  using typename BaseType::SourceSpaceType;
  using typename BaseType::VectorType;

  using AdvectionOperatorType = AdvectionOperatorImp;
  using EntropySolverType = EntropySolverImp;

  EntropyBasedMomentFvOperator(const AdvectionOperatorType& advection_operator, const EntropySolverType& entropy_solver)
    : advection_operator_(advection_operator)
    , entropy_solver_(entropy_solver)
  {}

  virtual bool linear() const override final
  {
    return false;
  }

  virtual const SourceSpaceType& source_space() const override final
  {
    return advection_operator_.source_space();
  }

  virtual const RangeSpaceType& range_space() const override final
  {
    return advection_operator_.range_space();
  }

  void apply(const VectorType& source, VectorType& range, const XT::Common::Parameter& param) const override final
  {
    //    static_assert(is_discrete_function<SourceType>::value || is_const_discrete_function<SourceType>::value,
    //                  "SourceType has to be derived from (Const)DiscreteFunction!");
    //    static_assert(is_discrete_function<RangeType>::value, "RangeType has to be derived from DiscreteFunction!");
    // solve optimization problems and regularize if necessary
    VectorType regularized = range;
    entropy_solver_.apply(source, regularized, param);

    std::fill(range.begin(), range.end(), 0.);
    advection_operator_.apply(regularized, range, param);
  }

  const AdvectionOperatorType& advection_operator_;
  const EntropySolverType& entropy_solver_;
}; // class EntropyBasedMomentFvOperatorNoReconstruction<...>


#if 0
template <class AdvectionOperatorImp,
          class ReconstructionOperatorImp,
          class RealizabilityLimiterImp,
          class RegularizationOperatorImp,
          class Traits = internal::EntropyBasedMomentFvOperatorTraits<AdvectionOperatorImp,
                                                                      ReconstructionOperatorImp,
                                                                      RealizabilityLimiterImp,
                                                                      RegularizationOperatorImp>>
class EntropyBasedMomentFvOperator : public OperatorInterface<Traits>
{
public:
  using AdvectionOperatorType = typename Traits::AdvectionOperatorType;
  using RegularizationOperatorType = typename Traits::RegularizationOperatorType;
  using ReconstructionOperatorType = typename Traits::ReconstructionOperatorType;
  using RealizabilityLimiterType = typename Traits::RealizabilityLimiterType;

  EntropyBasedMomentFvOperator(const AdvectionOperatorType& advection_operator,
                               const ReconstructionOperatorType& reconstruction_operator,
                               const RegularizationOperatorType& regularization_operator,
                               const RealizabilityLimiterType& realizability_limiter)
    : advection_operator_(advection_operator)
    , reconstruction_operator_(reconstruction_operator)
    , regularization_operator_(regularization_operator)
    , realizability_limiter_(realizability_limiter)
  {}

  template <class SourceType, class RangeType>
  void apply(const SourceType& source, RangeType& range, const XT::Common::Parameter& param) const
  {
    static_assert(is_discrete_function<SourceType>::value || is_const_discrete_function<SourceType>::value,
                  "SourceType has to be derived from (Const)DiscreteFunction!");
    static_assert(is_discrete_function<RangeType>::value, "RangeType has to be derived from DiscreteFunction!");
    using ReconstructedFunctionType = ReconstructedLocalizableFunction<typename SourceType::SpaceType::GridLayerType,
                                                                       typename SourceType::DomainFieldType,
                                                                       SourceType::dimDomain,
                                                                       typename SourceType::RangeFieldType,
                                                                       SourceType::dimRange,
                                                                       SourceType::dimRangeCols>;
    // solve optimization problems and regularize if necessary
    regularization_operator_.apply(source, range, param);

    // do reconstruction
    ReconstructedFunctionType reconstructed_function(range.space().grid_layer());
    reconstruction_operator_.apply(range, reconstructed_function, param);

    // perform realizability limiting
    realizability_limiter_.apply(range, reconstructed_function, param);

    std::fill(range.vector().begin(), range.vector().end(), 0.);
    advection_operator_.apply(reconstructed_function, range, param);
  }

  const AdvectionOperatorType& advection_operator_;
  const ReconstructionOperatorType& reconstruction_operator_;
  const RegularizationOperatorType& regularization_operator_;
  const RealizabilityLimiterType& realizability_limiter_;
}; // class EntropyBasedMomentFvOperator<...>
#endif


} // namespace GDT
} // namespace Dune

#endif // DUNE_GDT_OPERATORS_FV_ENTROPYBASED_HH
