// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017, 2020)
//   René Fritze     (2018 - 2020)
//   Tobias Leibner  (2017 - 2018, 2020)

#ifndef DUNE_XT_LA_EIGEN_SOLVER_HH
#define DUNE_XT_LA_EIGEN_SOLVER_HH

#include <complex>
#include <limits>

#include <dune/common/typetraits.hh>

#include <dune/common/dynvector.hh>
#include <dune/common/typetraits.hh>

#include <dune/xt/common/configuration.hh>
#include <dune/xt/common/vector.hh>
#include <dune/xt/common/type_traits.hh>
#include <dune/xt/la/exceptions.hh>
#include <dune/xt/la/container.hh>

namespace Dune::XT::LA {


/**
 * \brief A means to obtain available options at compile time.
 * \note  This class needs to be specialized for each MatrixType, the purpose of this variant is merely to document the
 *        expected functionality.
 */
template <class MatrixType,
          bool is_matrix =
              XT::Common::is_matrix<MatrixType>::value
              || (XT::Common::is_vector<MatrixType>::value && XT::Common::VectorAbstraction<MatrixType>::has_static_size
                  && XT::Common::VectorAbstraction<MatrixType>::static_size == 1)>
class EigenSolverOptions
{
  static_assert(AlwaysFalse<MatrixType>::value,
                "Please implement for given MatrixType and add the respective include below!");

  static std::vector<std::string> types();

  static Common::Configuration options(const std::string /*type*/ = "");
}; // class EigenSolverOptions


template <class MatrixType>
std::vector<std::string> eigen_solver_types(const MatrixType& /*matrix*/)
{
  return EigenSolverOptions<MatrixType>::types();
}


template <class MatrixType>
Common::Configuration eigen_solver_options(const MatrixType& /*matrix*/, const std::string& type = "")
{
  return EigenSolverOptions<MatrixType>::options(type);
}


template <class MatrixImp,
          bool is_matrix =
              XT::Common::is_matrix<MatrixImp>::value
              || (XT::Common::is_vector<MatrixImp>::value && XT::Common::VectorAbstraction<MatrixImp>::has_static_size
                  && XT::Common::VectorAbstraction<MatrixImp>::static_size == 1)>
class EigenSolver
{
  static_assert(AlwaysFalse<MatrixImp>::value,
                "Please implement for given MatrixType and add the respective include below!");

public:
  using MatrixType = MatrixImp;
  using FieldType = double;
  using RealMatrixType = int;
  using ComplexMatrixType = int;

  EigenSolver(const MatrixType& /*matrix*/, const std::string& /*type*/ = "")
  {
    static_assert(AlwaysFalse<MatrixType>::value,
                  "Please implement for given MatrixType and add the respective include below!");
  }

  EigenSolver(const MatrixType& /*matrix*/, const Common::Configuration& /*opts*/)
  {
    static_assert(AlwaysFalse<MatrixType>::value,
                  "Please implement for given MatrixType and add the respective include below!");
  }

  const Common::Configuration& options() const;

  const MatrixType& matrix() const;

  const std::vector<Common::complex_t<FieldType>>& eigenvalues() const;

  const std::vector<Common::real_t<FieldType>>& real_eigenvalues() const;

  const std::vector<Common::real_t<FieldType>>&
  min_eigenvalues(const size_t /*num_evs*/ = std::numeric_limits<size_t>::max()) const;

  const std::vector<Common::real_t<FieldType>>&
  max_eigenvalues(const size_t /*num_evs*/ = std::numeric_limits<size_t>::max()) const;

  const ComplexMatrixType& eigenvectors() const;

  const ComplexMatrixType& eigenvectors_inverse() const;

  const RealMatrixType& real_eigenvectors() const;

  const RealMatrixType& real_eigenvectors_inverse() const;
}; // class EigenSolver


template <class M>
EigenSolver<M> make_eigen_solver(const M& matrix, const std::string& type = "")
{
  return EigenSolver<M>(matrix, type);
}


template <class M>
EigenSolver<M> make_eigen_solver(const M& matrix, const XT::Common::Configuration& options)
{
  return EigenSolver<M>(matrix, options);
}


} // namespace Dune::XT::LA

#include "eigen-solver/default.hh"
#include "eigen-solver/eigen.hh"
#include "eigen-solver/fmatrix.hh"

#endif // DUNE_XT_LA_EIGEN_SOLVER_HH
