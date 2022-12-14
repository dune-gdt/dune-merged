// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2013 - 2014, 2016 - 2020)
//   René Fritze     (2013, 2015 - 2016, 2018 - 2020)
//   Tobias Leibner  (2014, 2018 - 2020)

#ifndef DUNE_XT_LA_SOLVER_HH
#define DUNE_XT_LA_SOLVER_HH

#include <string>
#include <vector>

#include <dune/xt/common/configuration.hh>
#include <dune/xt/common/matrix.hh>
#include <dune/xt/common/parallel/helper.hh>

#include <dune/xt/la/exceptions.hh>
#include <dune/xt/la/type_traits.hh>

namespace Dune::XT::LA {
namespace internal {


static constexpr size_t max_size_to_print = 5;


class SolverUtils
{
public:
  static void check_given(const std::string& type, const std::vector<std::string>& opts)
  {
    if (std::find(opts.begin(), opts.end(), type) == opts.end()) {
      std::stringstream ss;
      for (const auto& opt : opts)
        ss << opt << " ";
      DUNE_THROW(Common::Exceptions::configuration_error,
                 "Given type '" << type << "' not supported (see below for a list of supported ones). "
                                << "Call options() first!\n"
                                << ss.str());
    }
  }
};


} // namespace internal


template <class MatrixType, class CommunicatorType = SequentialCommunication>
class SolverOptions
{
  static_assert(AlwaysFalse<MatrixType>::value,
                "Please implement for given MatrixType and add the respective include below!");

public:
  static std::vector<std::string> types();

  static Common::Configuration options(const std::string /*type*/ = "");
}; // class SolverOptions


template <class MatrixImp, class CommunicatorType = SequentialCommunication>
class Solver
{
  static_assert(AlwaysFalse<MatrixImp>::value,
                "This is the unspecialized version of LA::Solver< ... >. Please include "
                "the correct header for your matrix implementation!");

public:
  using MatrixType = MatrixImp;

  Solver(const MatrixType& /*matrix*/, const CommunicatorType&)
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  Solver(const MatrixType& /*matrix*/)
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  static std::vector<std::string> types()
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  static Common::Configuration options(const std::string& /*type*/ = "")
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  /**
   *  Throws any of the above exceptions, if there was a problem. If none was thrown we beleive that a suitable solution
   *  was found (given the current options).
   */
  template <class RhsType, class SolutionType>
  void apply(const RhsType& /*rhs*/, SolutionType& /*solution*/) const
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  template <class RhsType, class SolutionType>
  void apply(const RhsType& /*rhs*/, SolutionType& /*solution*/, const std::string& /*type*/) const
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }

  template <class RhsType, class SolutionType>
  void apply(const RhsType& /*rhs*/, SolutionType& /*solution*/, const Common::Configuration& /*options*/) const
  {
    DUNE_THROW(NotImplemented,
               "This is the unspecialized version of LA::Solver< ... >. "
               "Please include the correct header for your matrix implementation '"
                   << Common::Typename<MatrixType>::value() << "'!");
  }
}; // class Solver


template <class M>
auto make_solver(const M& matrix)
{
  static_assert(is_matrix<M>::value || XT::Common::is_matrix<M>::value);
  return Solver<M>(matrix);
}


template <class M, class V, class... Args>
void solve(const M& A, const V& b, V& x, Args&&... args)
{
  static_assert(is_matrix<M>::value && is_vector<V>::value);
  make_solver(A).apply(b, x, std::forward<Args>(args)...);
}


// template <class M, class C>
// typename std::enable_if<is_matrix<M>::value, Solver<M, C>>::type make_solver(const M& matrix, const C&
// dof_comm)
//{
//  return Solver<M, C>(matrix, dof_comm);
//}


// template <class M, class V, class C, class... Args>
// typename std::enable_if<is_matrix<M>::value && is_vector<V>::value, void>::type
// solve(const M& A, const V& b, V& x, const C& dof_comm, Args&&... args)
//{
//  make_solver(A, dof_comm).apply(b, x, std::forward<Args>(args)...);
//}


template <class M, class V, class S>
typename VectorInterface<V, S>::derived_type
solve(const MatrixInterface<M, S>& A,
      const VectorInterface<V, S>& b,
      const Common::Configuration& opts = SolverOptions<typename MatrixInterface<M, S>::derived_type>::options())
{
  typename VectorInterface<V, S>::derived_type x(A.cols(), 0.);
  Solver<typename MatrixInterface<M, S>::derived_type> solver(A.as_imp());
  solver.apply(b.as_imp(), x, opts);
  return x;
}


} // namespace Dune::XT::LA

#include "solver/common.hh"
#include "solver/dense.hh"
#include "solver/eigen.hh"
#include "solver/istl.hh"
#include "solver/view.hh"

#endif // DUNE_XT_LA_SOLVER_HH
