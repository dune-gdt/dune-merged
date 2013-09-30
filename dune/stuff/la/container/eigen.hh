#ifndef DUNE_STUFF_LA_CONTAINER_EIGEN_HH
#define DUNE_STUFF_LA_CONTAINER_EIGEN_HH

#if HAVE_EIGEN

#include <algorithm>
#include <memory>
#include <type_traits>

#include <Eigen/Core>
#include <Eigen/SparseCore>

#include <dune/stuff/aliases.hh>
#include <dune/stuff/common/ranges.hh>

#include "interface.hh"
#include "pattern.hh"

namespace Dune {
namespace Stuff {
namespace LA {


template <class Traits>
class EigenContainerInterface : public ContainerInterface<Traits>
{
public:
  typedef typename Traits::derived_type derived_type;
}; // class EigenInterface


template <class Traits>
class EigenMatrixInterface : public EigenContainerInterface<Traits>
{
public:
  typedef typename Traits::derived_type derived_type;
}; // class EigenMatrixInterface


template <class Traits>
class EigenVectorInterface : public EigenContainerInterface<Traits>
{
public:
  typedef typename Traits::derived_type derived_type;
}; // class EigenVectorInterface


template <class ElementType>
class EigenRowMajorSparseMatrix;


template <class ElementImp>
class EigenDenseMatrix;


template <class ElementImp>
class EigenDenseVector;


template <class ElementImp = double>
class EigenRowMajorSparseMatrixTraits
{
public:
  typedef ElementImp ElementType;
  typedef EigenRowMajorSparseMatrix<ElementType> derived_type;
  typedef typename ::Eigen::SparseMatrix<ElementType, ::Eigen::RowMajor> BackendType;
}; // class RowMajorSparseMatrixTraits


template <class ElementImp = double>
class EigenRowMajorSparseMatrix : public MatrixInterface<EigenRowMajorSparseMatrixTraits<ElementImp>>,
                                  public EigenMatrixInterface<EigenRowMajorSparseMatrixTraits<ElementImp>>
{
public:
  typedef EigenRowMajorSparseMatrix<ElementImp> ThisType;
  typedef EigenRowMajorSparseMatrixTraits<ElementImp> Traits;
  typedef typename Traits::BackendType BackendType;
  typedef typename Traits::ElementType ElementType;

  EigenRowMajorSparseMatrix()
  {
  }

  EigenRowMajorSparseMatrix(const ThisType& _other)
    : eigenMatrix_(_other.backend())
  {
  }

  EigenRowMajorSparseMatrix(const BackendType& _otherEigenMatrix)
    : eigenMatrix_(_otherEigenMatrix)
  {
  }

  EigenRowMajorSparseMatrix(const size_t _rows, const size_t _cols, const SparsityPatternDefault& _pattern)
    : eigenMatrix_(_rows, _cols)
  {
    assert(size_t(_pattern.size()) == _rows && "Given pattern too short!");
    for (size_t row = 0; row < size_t(_pattern.size()); ++row) {
      eigenMatrix_.startVec(row);
      for (auto& column : _pattern.inner(row)) {
        eigenMatrix_.insertBackByOuterInner(row, column);
      }
      // create diagonal entry (insertBackByOuterInner() can not handle empty rows)
      if (_pattern.inner(row).size() == 0)
        eigenMatrix_.insertBackByOuterInner(row, row);
    }
    eigenMatrix_.finalize();
    eigenMatrix_.makeCompressed();
  } // RowMajorSparseMatrix(...)

  /**
   *  \attention  This is not optimal, since we create a new triplet vector inbetween!
   */
  EigenRowMajorSparseMatrix(const ThisType& other, const Dune::Stuff::LA::SparsityPatternDefault& pattern)
    : eigenMatrix_(other.rows(), other.cols())
  {
    typedef ::Eigen::Triplet<ElementType> TripletType;
    std::vector<TripletType> triplets;
    triplets.reserve(other.nonZeros());
    for (size_t row = 0; row < pattern.size(); ++row)
      for (size_t col : pattern.inner(row))
        triplets.emplace_back(row, col, other.get(row, col));
    eigenMatrix_.setFromTriplets(triplets.begin(), triplets.end());
  }

  ThisType& operator=(const ThisType& _other)
  {
    eigenMatrix_ = _other.backend();
    return *this;
  }

  ThisType& operator=(const BackendType& _otherEigenMatrix)
  {
    eigenMatrix_ = _otherEigenMatrix;
    return *this;
  }

  size_t rows() const
  {
    return eigenMatrix_.rows();
  }

  size_t cols() const
  {
    return eigenMatrix_.cols();
  }

  void add(const size_t ii, const size_t jj, const ElementType& val)
  {
    assert(these_are_valid_indices(ii, jj));
    eigenMatrix_.coeffRef(ii, jj) += val;
  }

  void set(const size_t ii, const size_t jj, const ElementType& val)
  {
    assert(these_are_valid_indices(ii, jj));
    eigenMatrix_.coeffRef(ii, jj) = val;
  }

  const ElementType get(const size_t ii, const size_t jj) const
  {
    assert(ii < rows());
    assert(jj < cols());
    return eigenMatrix_.coeff(ii, jj);
  }

  void unitRow(const size_t row)
  {
    assert(row < rows());
    clearRow(row);
    set(row, row, ElementType(1));
  }

  void unitCol(const size_t col)
  {
    assert(col < cols());
    for (size_t row : DSC::valueRange(rows())) {
      if (row == col)
        set(row, col, ElementType(1));
      else if (eigenMatrix_.coeff(row, col) != ElementType(0))
        set(row, col, ElementType(0));
    }
  }

  void clearRow(const size_t ii)
  {
    assert(ii < rows());
    auto row = eigenMatrix_.row(ii);
    row *= ElementType(0);
  }

  BackendType& backend()
  {
    return eigenMatrix_;
  }

  const BackendType& backend() const
  {
    return eigenMatrix_;
  }

private:
  bool these_are_valid_indices(const size_t ii, const size_t jj)
  {
    if (ii >= rows())
      return false;
    if (jj >= cols())
      return false;
    for (size_t row = 0; row < eigenMatrix_.outerSize(); ++row) {
      for (typename BackendType::InnerIterator row_it(eigenMatrix_, row); row_it; ++row_it) {
        const size_t col = row_it.col();
        if ((ii == row) && (jj == col))
          return true;
        else if ((row > ii) && (col > jj))
          return false;
      }
    }
    return false;
  } // ... these_are_valid_indices(...)

  BackendType eigenMatrix_;
}; // class EigenRowMajorSparseMatrix


template <class ElementImp = double>
class EigenDenseMatrixTraits
{
public:
  typedef ElementImp ElementType;
  typedef EigenDenseMatrix<ElementType> derived_type;
  typedef typename ::Eigen::Matrix<ElementType, ::Eigen::Dynamic, ::Eigen::Dynamic> BackendType;
}; // class DenseMatrixTraits


/**
 *  \brief  A dense matrix using the eigen backend
 *          If the nontrivial constructors is used, the storage is initialized with zero.
 */
template <class ElementImp = double>
class EigenDenseMatrix : public MatrixInterface<EigenDenseMatrixTraits<ElementImp>>,
                         public EigenMatrixInterface<EigenDenseMatrixTraits<ElementImp>>
{
public:
  typedef EigenDenseMatrix<ElementImp> ThisType;
  typedef EigenDenseMatrixTraits<ElementImp> Traits;
  typedef typename Traits::BackendType BackendType;
  typedef typename Traits::ElementType ElementType;

  EigenDenseMatrix()
  {
  }

  EigenDenseMatrix(const ThisType& other)
    : eigenMatrix_(other.backend())
  {
  }

  EigenDenseMatrix(const BackendType& otherEigenMatrix)
    : eigenMatrix_(otherEigenMatrix)
  {
  }

  EigenDenseMatrix(const EigenRowMajorSparseMatrix<ElementType>& other)
    : eigenMatrix_(other.backend())
  {
  }

  EigenDenseMatrix(const EigenDenseVector<ElementType>& other)
    : eigenMatrix_(other.backend())
  {
  }

  EigenDenseMatrix(const size_t _rows, const size_t _cols)
    : eigenMatrix_(_rows, _cols)
  {
    eigenMatrix_.setZero(_rows, _cols);
  }

  ThisType& operator=(const ThisType& _other)
  {
    eigenMatrix_ = _other.backend();
    return *this;
  }

  ThisType& operator=(const BackendType& _otherEigenMatrix)
  {
    eigenMatrix_ = _otherEigenMatrix;
    return *this;
  }

  size_t rows() const
  {
    return eigenMatrix_.rows();
  }

  size_t cols() const
  {
    return eigenMatrix_.cols();
  }

  void add(const size_t ii, const size_t jj, const ElementType& val)
  {
    assert(ii < rows());
    assert(jj < cols());
    eigenMatrix_(ii, jj) += val;
  }

  void set(const size_t ii, const size_t jj, const ElementType& val)
  {
    assert(ii < rows());
    assert(jj < cols());
    eigenMatrix_(ii, jj) = val;
  }

  const ElementType get(const size_t ii, const size_t jj) const
  {
    assert(ii < rows());
    assert(jj < cols());
    return eigenMatrix_(ii, jj);
  }

  BackendType& backend()
  {
    return eigenMatrix_;
  }

  const BackendType& backend() const
  {
    return eigenMatrix_;
  }

private:
  BackendType eigenMatrix_;
}; // class EigenDenseMatrix


template <class ElementImp = double>
class EigenDenseVectorTraits
{
public:
  typedef ElementImp ElementType;
  typedef EigenDenseVector<ElementType> derived_type;
  typedef typename ::Eigen::Matrix<ElementType, ::Eigen::Dynamic, 1> BackendType;
}; // class DenseVectorTraits

/**
 *  \brief  A dense vector using the eigen backend
 *          If the nontrivial constructors is used, the storage is initialized with zero.
 */
template <class ElementImp = double>
class EigenDenseVector : public VectorInterface<EigenDenseVectorTraits<ElementImp>>,
                         public EigenVectorInterface<EigenDenseVectorTraits<ElementImp>>
{
public:
  typedef EigenDenseVector<ElementImp> ThisType;
  typedef EigenDenseVectorTraits<ElementImp> Traits;
  typedef typename Traits::BackendType BackendType;
  typedef typename Traits::ElementType ElementType;

  EigenDenseVector()
  {
  }

  EigenDenseVector(const ThisType& other)
    : eigenVector_(other.backend())
  {
  }

  EigenDenseVector(const BackendType& otherEigenVector)
    : eigenVector_(otherEigenVector)
  {
  }

  EigenDenseVector(const size_t _size)
    : eigenVector_(_size)
  {
    eigenVector_.setZero(_size);
  }

  EigenDenseVector(const size_t _size, const ElementType value)
    : eigenVector_(_size)
  {
    eigenVector_.setOnes();
    eigenVector_ *= value;
  }

  ThisType& operator=(const ThisType& other)
  {
    eigenVector_ = other.backend();
    return *this;
  }

  ThisType& operator=(const BackendType& _otherEigenVector)
  {
    eigenVector_ = _otherEigenVector;
    return *this;
  }

  size_t size() const
  {
    return eigenVector_.size();
  }

  void add(const size_t ii, const ElementType& val)
  {
    assert(ii < size());
    eigenVector_(ii) += val;
  }

  void set(const size_t ii, const ElementType& val)
  {
    assert(ii < size());
    eigenVector_(ii) = val;
  }

  const ElementType get(const size_t ii) const
  {
    assert(ii < size());
    return eigenVector_(ii);
  }

  BackendType& backend()
  {
    return eigenVector_;
  }

  const BackendType& backend() const
  {
    return eigenVector_;
  }

private:
  BackendType eigenVector_;
}; // class DenseVector


template <class T>
class EigenMappedDenseVector;

template <class ElementImp = double>
class EigenMappedDenseVectorTraits
{
  typedef typename ::Eigen::Matrix<ElementImp, ::Eigen::Dynamic, 1> PlainBackendType;

public:
  typedef ElementImp ElementType;
  typedef EigenMappedDenseVector<ElementType> derived_type;
  typedef Eigen::Map<PlainBackendType> BackendType;
};

/**
 *  \brief  A EigenMap backed Vector interface wrapping double*
 *
 */
template <class ElementImp = double>
class EigenMappedDenseVector : public VectorInterface<EigenMappedDenseVectorTraits<ElementImp>>,
                               public EigenVectorInterface<EigenMappedDenseVectorTraits<ElementImp>>
{
public:
  typedef EigenMappedDenseVector<ElementImp> ThisType;
  typedef EigenMappedDenseVectorTraits<ElementImp> Traits;
  typedef typename Traits::BackendType BackendType;
  typedef typename Traits::ElementType ElementType;

  EigenMappedDenseVector(ElementImp* data, size_t data_size)
    : eigenVector_(data, data_size)
  {
    static_assert(std::is_same<ElementType, double>::value, "undefined behaviour for non-double data");
  }

  virtual ~EigenMappedDenseVector()
  {
  }

  EigenMappedDenseVector() = delete;

  size_t size() const
  {
    return eigenVector_.size();
  }

  void add(const size_t ii, const ElementType& val)
  {
    assert(ii < size());
    eigenVector_(ii) += val;
  }

  void set(const size_t ii, const ElementType& val)
  {
    assert(ii < size());
    eigenVector_(ii) = val;
  }

  const ElementType get(const size_t ii) const
  {
    assert(ii < size());
    return eigenVector_(ii);
  }

  BackendType& backend()
  {
    return eigenVector_;
  }

  const BackendType& backend() const
  {
    return eigenVector_;
  }

private:
  BackendType eigenVector_;
}; // class DenseVector


} // namespace LA
} // namespace Stuff
} // namespace Dune

#endif // HAVE_EIGEN

#endif // DUNE_STUFF_LA_CONTAINER_EIGEN_HH
