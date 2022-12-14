// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017 - 2018, 2020)
//   René Fritze     (2018 - 2020)
//   Tobias Leibner  (2018 - 2020)

#ifndef DUNE_XT_GRID_FUNCTORS_BOUNDARY_DETECTOR_HH
#define DUNE_XT_GRID_FUNCTORS_BOUNDARY_DETECTOR_HH

#include <dune/xt/common/parallel/threadstorage.hh>

#include <dune/xt/grid/boundaryinfo/interfaces.hh>
#include <dune/xt/grid/intersection.hh>
#include <dune/xt/grid/print.hh>

#include "interfaces.hh"

namespace Dune::XT::Grid {


template <class GL>
class BoundaryDetectorFunctor
  : public IntersectionFunctor<GL>
  , public Common::ThreadResultPropagator<BoundaryDetectorFunctor<GL>, size_t>
{
  using BaseType = IntersectionFunctor<GL>;
  using Propagator = Common::ThreadResultPropagator<BoundaryDetectorFunctor<GL>, size_t>;

public:
  using typename BaseType::ElementType;
  using typename BaseType::GridViewType;
  using typename BaseType::IntersectionType;

  /**
   * \attention Takes ownership of boundary_type_ptr, do not delete manually!
   */
  BoundaryDetectorFunctor(const BoundaryInfo<IntersectionType>& boundary_info,
                          BoundaryType*&& boundary_type_ptr,
                          const std::string& logging_prefix = "",
                          const std::array<bool, 3>& logging_state = Common::default_logger_state())
    : BaseType(logging_prefix.empty() ? "BoundaryDetectorFunctor" : logging_prefix, logging_state)
    , Propagator(this)
    , boundary_info_(boundary_info)
    , boundary_type_(boundary_type_ptr)
    , found_(0)
  {
    LOG_(debug) << "BoundaryDetectorFunctor(boundary_info=" << boundary_info_ << ", boundary_type=" << *boundary_type_
                << ")" << std::endl;
  }

  BoundaryDetectorFunctor(const BoundaryInfo<IntersectionType>& boundary_info,
                          const BoundaryType& boundary_type,
                          const std::string& logging_prefix = "",
                          const std::array<bool, 3>& logging_state = Common::default_logger_state())
    : BaseType(logging_prefix.empty() ? "BoundaryDetectorFunctor" : logging_prefix, logging_state)
    , Propagator(this)
    , boundary_info_(boundary_info)
    , boundary_type_(boundary_type.copy())
    , found_(0)
  {
    LOG_(debug) << "BoundaryDetectorFunctor(boundary_info=" << boundary_info_ << ", boundary_type=" << *boundary_type_
                << ")" << std::endl;
  }

  BoundaryDetectorFunctor(const BoundaryDetectorFunctor& other) = default;

  void prepare() final
  {
    found_ = 0;
  }

  size_t compute_locally(const IntersectionType& intersection,
                         const ElementType& /*inside_element*/,
                         const ElementType& /*outside_element*/)
  {
    LOG_(debug) << "compute_locally(intersection=" << print(intersection)
                << "):\n  boundary_info_.type(intersection) = " << boundary_info_.type(intersection) << ", returning "
                << size_t(boundary_info_.type(intersection) == *boundary_type_) << std::endl;
    return boundary_info_.type(intersection) == *boundary_type_;
  }

  void apply_local(const IntersectionType& intersection,
                   const ElementType& inside_element,
                   const ElementType& outside_element) final
  {
    LOG_(debug) << "apply_local(intersection=" << print(intersection) << "): calling compute_locally()" << std::endl;
    found_ += compute_locally(intersection, inside_element, outside_element);
  }

  size_t result() const
  {
    return found_;
  }

  void finalize() final
  {
    Propagator::finalize_imp();
  }

  BaseType* copy() final
  {
    return Propagator::copy_imp();
  }

protected:
  void set_result(size_t res)
  {
    found_ = res;
  }

private:
  friend Propagator;

  const BoundaryInfo<IntersectionType>& boundary_info_;
  const std::shared_ptr<BoundaryType> boundary_type_;
  size_t found_;
}; // class BoundaryDetectorFunctor


} // namespace Dune::XT::Grid


#endif // DUNE_XT_GRID_FUNCTORS_BOUNDARY_DETECTOR_HH
