// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2014, 2016 - 2017)
//   René Fritze     (2014 - 2016, 2018 - 2019)
//   Tobias Leibner  (2014 - 2020)

#ifndef DUNE_XT_TEST_LA_CONTAINER_HH
#define DUNE_XT_TEST_LA_CONTAINER_HH

#include <complex>
#include <memory>
#include <type_traits>

#include <dune/xt/common/exceptions.hh>
#include <dune/xt/common/float_cmp.hh>
#include <dune/xt/common/matrix.hh>
#include <dune/xt/common/vector.hh>

#include <dune/xt/la/container/common.hh>
#include <dune/xt/la/container/eigen.hh>
#include <dune/xt/la/container/eye-matrix.hh>
#include <dune/xt/la/container/interfaces.hh>
#include <dune/xt/la/container/istl.hh>
#include <dune/xt/la/container.hh>

template <class ContainerImp>
class ContainerFactory
{
public:
  static ContainerImp create(const size_t /*size*/)
  {
    static_assert(Dune::AlwaysFalse<ContainerImp>::value, "Please specialize this class for this ContainerImp!");
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonDenseVector<S>>
{
public:
  static Dune::XT::LA::CommonDenseVector<S> create(const size_t size)
  {
    return Dune::XT::LA::CommonDenseVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::DynamicVector<S>>
{
public:
  static Dune::DynamicVector<S> create(const size_t size)
  {
    return Dune::DynamicVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::Common::FieldVector<S, 4>>
{
public:
  static Dune::XT::Common::FieldVector<S, 4> create(const size_t size)
  {
    DUNE_THROW_IF(size != 4, Dune::NotImplemented, "");
    return Dune::XT::Common::FieldVector<S, 4>(S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::Common::FieldVector<S, 10>>
{
public:
  static Dune::XT::Common::FieldVector<S, 10> create(const size_t size)
  {
    DUNE_THROW_IF(size != 10, Dune::NotImplemented, "");
    return Dune::XT::Common::FieldVector<S, 10>(S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonSparseVector<S>>
{
public:
  static Dune::XT::LA::CommonSparseVector<S> create(const size_t size)
  {
    return Dune::XT::LA::CommonSparseVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::Common::FieldMatrix<S, 10, 10>>
{
public:
  static Dune::XT::Common::FieldMatrix<S, 10, 10> create(const size_t size)
  {
    DUNE_THROW_IF(size != 10, Dune::NotImplemented, "");
    Dune::XT::Common::FieldMatrix<S, 10, 10> matrix(0);
    for (size_t ii = 0; ii < size; ++ii)
      matrix[ii][ii] = 1;
    return matrix;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonDenseMatrix<S>>
{
public:
  static Dune::XT::LA::CommonDenseMatrix<S> create(const size_t size)
  {
    Dune::XT::LA::CommonDenseMatrix<S> matrix(size, size);
    for (size_t ii = 0; ii < size; ++ii)
      matrix.unit_row(ii);
    return matrix;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonSparseMatrixCsr<S>>
{
public:
  static Dune::XT::LA::CommonSparseMatrixCsr<S> create(const size_t size)
  {
    auto ret = Dune::XT::LA::eye_matrix<Dune::XT::LA::CommonSparseMatrixCsr<S>>(size, size);
    return ret;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonSparseMatrixCsc<S>>
{
public:
  static Dune::XT::LA::CommonSparseMatrixCsc<S> create(const size_t size)
  {
    auto ret = Dune::XT::LA::eye_matrix<Dune::XT::LA::CommonSparseMatrixCsc<S>>(size, size);
    return ret;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonSparseOrDenseMatrixCsr<S>>
{
public:
  static Dune::XT::LA::CommonSparseOrDenseMatrixCsr<S> create(const size_t size)
  {
    auto ret = Dune::XT::LA::eye_matrix<Dune::XT::LA::CommonSparseOrDenseMatrixCsr<S>>(size, size);
    return ret;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::CommonSparseOrDenseMatrixCsc<S>>
{
public:
  static Dune::XT::LA::CommonSparseOrDenseMatrixCsc<S> create(const size_t size)
  {
    auto ret = Dune::XT::LA::eye_matrix<Dune::XT::LA::CommonSparseOrDenseMatrixCsc<S>>(size, size);
    return ret;
  }
};


template <class S>
class ContainerFactory<Dune::XT::LA::IstlDenseVector<S>>
{
public:
  static Dune::XT::LA::IstlDenseVector<S> create(const size_t size)
  {
    return Dune::XT::LA::IstlDenseVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::IstlRowMajorSparseMatrix<S>>
{
public:
  static Dune::XT::LA::IstlRowMajorSparseMatrix<S> create(const size_t size)
  {
    Dune::XT::LA::SparsityPatternDefault pattern(size);
    for (size_t ii = 0; ii < size; ++ii)
      pattern.inner(ii).push_back(ii);
    Dune::XT::LA::IstlRowMajorSparseMatrix<S> matrix(size, size, pattern);
    for (size_t ii = 0; ii < size; ++ii)
      matrix.unit_row(ii);
    return matrix;
  }
};


#if HAVE_EIGEN
template <class S>
class ContainerFactory<Dune::XT::LA::EigenDenseVector<S>>
{
public:
  static Dune::XT::LA::EigenDenseVector<S> create(const size_t size)
  {
    return Dune::XT::LA::EigenDenseVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::EigenMappedDenseVector<S>>
{
public:
  static Dune::XT::LA::EigenMappedDenseVector<S> create(const size_t size)
  {
    return Dune::XT::LA::EigenMappedDenseVector<S>(size, S(1));
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::EigenDenseMatrix<S>>
{
public:
  static Dune::XT::LA::EigenDenseMatrix<S> create(const size_t size)
  {
    Dune::XT::LA::EigenDenseMatrix<S> matrix(size, size);
    for (size_t ii = 0; ii < size; ++ii)
      matrix.unit_row(ii);
    return matrix;
  }
};

template <class S>
class ContainerFactory<Dune::XT::LA::EigenRowMajorSparseMatrix<S>>
{
public:
  static Dune::XT::LA::EigenRowMajorSparseMatrix<S> create(const size_t size)
  {
    Dune::XT::LA::SparsityPatternDefault pattern(size);
    for (size_t ii = 0; ii < size; ++ii)
      pattern.inner(ii).push_back(ii);
    Dune::XT::LA::EigenRowMajorSparseMatrix<S> matrix(size, size, pattern);
    for (size_t ii = 0; ii < size; ++ii)
      matrix.unit_row(ii);
    return matrix;
  }
};
#endif // HAVE_EIGEN

#define EXPECT_DOUBLE_OR_COMPLEX_EQ(expected, actual)                                                                  \
  {                                                                                                                    \
    const auto expected_val = expected; /* avoids errors if macro is called e.g. with expected++ */                    \
    const auto actual_val = actual;                                                                                    \
    EXPECT_DOUBLE_EQ(std::real(expected_val), std::real(actual_val));                                                  \
    EXPECT_DOUBLE_EQ(std::imag(expected_val), std::imag(actual_val));                                                  \
  }

#endif // DUNE_XT_TEST_LA_CONTAINER_HH
