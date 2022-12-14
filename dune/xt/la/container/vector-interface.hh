// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Barbara Verfürth (2015)
//   Felix Schindler  (2012 - 2019)
//   René Fritze      (2013 - 2020)
//   Tobias Leibner   (2014, 2017 - 2020)

#ifndef DUNE_XT_LA_CONTAINER_VECTOR_INTERFACE_HH
#define DUNE_XT_LA_CONTAINER_VECTOR_INTERFACE_HH

#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <complex>

#include <dune/common/bigunsignedint.hh>

#include <dune/xt/common/crtp.hh>
#include <dune/xt/common/exceptions.hh>
#include <dune/xt/common/float_cmp.hh>
#include <dune/xt/common/math.hh>
#include <dune/xt/common/type_traits.hh>
#include <dune/xt/common/vector.hh>

#include <dune/xt/la/type_traits.hh>

#include <dune/xt/la/exceptions.hh>

#include "container-interface.hh"
#include "vector-interface-internal.hh"

namespace Dune::XT::LA {
namespace internal {


template <class ScalarImp,
          class derived_imp,
          class BackendImp,
          Backends backend_imp,
          Backends dense_matrix_imp,
          Backends sparse_matrix_imp>
class VectorTraitsBase
{
public:
  using ScalarType = typename Dune::FieldTraits<ScalarImp>::field_type;
  using RealType = typename Dune::FieldTraits<ScalarImp>::real_type;
  using MutexesType = std::vector<std::mutex>;
  using DataType = ScalarType;
  using BackendType = BackendImp;
  using derived_type = derived_imp;
  static constexpr Backends backend_type = backend_imp;
  static constexpr Backends dense_matrix_type = dense_matrix_imp;
  static constexpr Backends sparse_matrix_type = sparse_matrix_imp;
};


} // namespace internal


template <class Traits, class ScalarImp = typename Traits::ScalarType>
class VectorInterface : public ContainerInterface<Traits, ScalarImp>
{
  using BaseType = ContainerInterface<Traits, ScalarImp>;

public:
  using typename BaseType::derived_type;
  using typename BaseType::RealType;
  using typename BaseType::ScalarType;
  static constexpr Backends dense_matrix_type = Traits::dense_matrix_type;
  static constexpr Backends sparse_matrix_type = Traits::sparse_matrix_type;
  using const_iterator = internal::VectorInputIterator<Traits, ScalarType>;
  using iterator = internal::VectorOutputIterator<Traits, ScalarType>;
  friend const_iterator;
  friend iterator;
  static_assert(std::is_same<ScalarType, typename Traits::ScalarType>::value);

  ~VectorInterface() override = default;

  template <class Vector>
  std::enable_if_t<Common::is_vector<Vector>::value, derived_type&> assign_from(const Vector& other)
  {
    this->resize(other.size());
    for (size_t ii = 0; ii < other.size(); ++ii)
      this->set_entry(ii, other[ii]);
    return this->as_imp();
  }

  template <class T, class S>
  derived_type& operator=(const VectorInterface<T, S>& other)
  {
    for (size_t ii = 0; ii < size(); ++ii)
      set_entry(ii, other.get_entry(ii));
    return this->as_imp();
  }

  /// \name Have to be implemented by a derived class in addition to the ones required by ContainerInterface!
  /// \{

  /**
   * \brief   The size of the vector.
   * \return  The size of the vector.
   */
  inline size_t size() const
  {
    CHECK_CRTP(this->as_imp().size());
    return this->as_imp().size();
  }

  /**
   * \note This is supposed to do nothing if new_size == size()
   */
  inline void resize(const size_t new_size)
  {
    CHECK_AND_CALL_CRTP(this->as_imp().resize(new_size));
  }

  /**
   * \brief Add a scalar to the iith entry.
   */
  inline void add_to_entry(const size_t ii, const ScalarType& value)
  {
    CHECK_AND_CALL_CRTP(this->as_imp().add_to_entry(ii, value));
  }

  /**
   * \brief Set the iith entry to given scalar.
   */
  inline void set_entry(const size_t ii, const ScalarType& value)
  {
    CHECK_AND_CALL_CRTP(this->as_imp().set_entry(ii, value));
  }

  /**
   * \brief Get the iith entry.
   */
  inline ScalarType get_entry(const size_t ii) const
  {
    CHECK_CRTP(this->as_imp().get_entry(ii));
    return this->as_imp().get_entry(ii);
  }

protected:
  /**
   * The purpose of get_unchecked_ref is to allow direct access to the underlying data without any checks (regarding cow
   * or thread safety). This allows default implementations in the interface with locking prior to for-loops.
   */
  inline ScalarType& get_unchecked_ref(const size_t ii)
  {
    CHECK_CRTP(this->as_imp().get_unchecked_ref(ii));
    return this->as_imp().get_unchecked_ref(ii);
  }

  inline const ScalarType& get_unchecked_ref(const size_t ii) const
  {
    CHECK_CRTP(this->as_imp().get_unchecked_ref(ii));
    return this->as_imp().get_unchecked_ref(ii);
  }

public:
  /// \}
  /// \name Provided by the interface for convenience!
  /// \note Those marked as virtual may be implemented more efficiently in a derived class!
  /// \{

  virtual void set_all(const ScalarType& val)
  {
    const auto sz = size();
    for (size_t ii = 0; ii < sz; ++ii)
      set_entry(ii, val);
  }

  virtual bool valid() const
  {
    for (const auto& val : *this) {
      if (Common::isnan(val) || Common::isinf(val))
        return false;
    }
    return true;
  } // ... valid()

  /**
   * \brief Get writable reference to the iith entry.
   */
  inline ScalarType& operator[](const size_t ii)
  {
    CHECK_CRTP(this->as_imp()[ii]);
    return this->as_imp()[ii];
  }

  /**
   * \brief Get read-only reference to the iith entry.
   */
  inline const ScalarType& operator[](const size_t ii) const
  {
    CHECK_CRTP(this->as_imp()[ii]);
    return this->as_imp()[ii];
  }

  virtual ScalarType min() const
  {
    return complex_switch::min(this->as_imp());
  }

  virtual ScalarType max() const
  {
    return complex_switch::max(this->as_imp());
  }

  virtual ScalarType mean() const
  {
    ScalarType ret = 0.0;
    for (const auto& element : *this)
      ret += element;
    ret /= size();
    return ret;
  } // ... mean()

  /**
   *  \brief  The maximum absolute value of the vector.
   *  \return A pair of the highest index at which the maximum is attained and the absolute maximum value.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual std::pair<size_t, RealType> amax() const
  {
    using XT::Common::abs;
    auto result = std::make_pair(size_t(0), RealType(0));
    for (size_t ii = 0; ii < size(); ++ii) {
      const auto value = abs(get_unchecked_ref(ii));
      if (value > result.second) {
        result.first = ii;
        result.second = value;
      }
    }
    return result;
  } // ... amax(...)

  /**
   *  \brief  Check vectors for equality.
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   *  \param  other   A vector of same size to compare with.
   *  \param  epsilon See Dune::FloatCmp.
   *  \return Truth value of the comparison.
   *  \see    Dune::Common::FloatCmp
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual bool almost_equal(const derived_type& other,
                            const ScalarType epsilon = Common::FloatCmp::DefaultEpsilon<ScalarType>::value()) const
  {
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    // If you get an error here, you might be missing the VectorAbstraction for you type!
    return Common::FloatCmp::eq(this->as_imp(), other, epsilon);
  } // ... almost_equal(...)

  /**
   *  \brief  Check vectors for equality (variant for arbitrary derived combinations).
   *          Equality of two vectors is defined as in Dune::FloatCmp componentwise.
   *  \param  other   A vector of same size to compare with.
   *  \param  epsilon See Dune::FloatCmp.
   *  \return Truth value of the comparison.
   *  \see    Dune::Common::FloatCmp
   */
  template <class T>
  bool almost_equal(const VectorInterface<T>& other,
                    const ScalarType epsilon = Common::FloatCmp::DefaultEpsilon<ScalarType>::value()) const
  {
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    // If you get an error here, you might be missing the VectorAbstraction for you type!
    return Common::FloatCmp::eq(this->as_imp(), other.as_imp(), epsilon);
  } // ... almost_equal(...)

  /**
   *  \brief  Computes the scalar products between two vectors.
   *  \param  other The second factor.
   *  \return The scalar product.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual ScalarType dot(const derived_type& other) const
  {
    return complex_switch::dot(this->as_imp(), other);
  }

  /**
   *  \brief  The l1-norm of the vector.
   *  \return The l1-norm of the vector.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual RealType l1_norm() const
  {
    using XT::Common::abs;
    RealType result = 0;
    for (size_t ii = 0; ii < size(); ++ii)
      result += abs(get_unchecked_ref(ii));
    return result;
  } // ... l1_norm(...)

  // for compatibility with dune core modules
  RealType one_norm() const
  {
    return l1_norm();
  }

  /**
   *  \brief  The l2-norm of the vector.
   *  \return The l2-norm of the vector.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual RealType l2_norm() const
  {
    using std::sqrt;
    using XT::Common::abs;
    return sqrt(abs(dot(this->as_imp()))); // std::abs is only needed for the right return type:
    // v.dot(v) should always be a ScalarType with zero imaginary part
  }

  // for compatibility with dune core modules
  RealType two_norm() const
  {
    return l2_norm();
  }

  /**
   *  \brief  The l-infintiy-norm of the vector.
   *  \return The l-infintiy-norm of the vector.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual RealType sup_norm() const
  {
    return amax().second;
  }

  // for compatibility with dune core modules
  RealType inf_norm() const
  {
    return sup_norm();
  }

  virtual ScalarType standard_deviation() const
  {
    using std::pow;
    using std::sqrt;
    const ScalarType mu = mean();
    ScalarType sigma = 0.0;
    for (const auto& x_i : *this)
      sigma += pow(x_i - mu, 2);
    sigma /= size();
    return sqrt(sigma);
  } // ... standard_deviation(...)

  /**
   *  \brief  Adds two vectors.
   *  \param  other   The right summand.
   *  \param  result  Vector to write the result of this + other to
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   *  \note   If you are looking for the old (now deprecated) add() method, \see add_to_entry().
   */
  virtual void add(const derived_type& other, derived_type& result) const
  {
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    if (result.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of result (" << result.size() << ") does not match the size of this (" << size() << ")!");
    for (size_t ii = 0; ii < size(); ++ii)
      result.set_entry(ii, get_unchecked_ref(ii) + other.get_unchecked_ref(ii));
  } // ... add(...)

  /**
   *  \brief  Adds two vectors.
   *  \param  other The right summand.
   *  \return The sum of this and other.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   *  \note   If you are looking for the old (now deprecated) add() method, \see add_to_entry().
   */
  virtual derived_type add(const derived_type& other) const
  {
    derived_type result = this->copy();
    result.iadd(other);
    return result;
  } // ... add(...)

  /**
   *  \brief  Inplace variant of add().
   *  \param  other The right summand.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual void iadd(const derived_type& other)
  {
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    for (size_t ii = 0; ii < size(); ++ii)
      add_to_entry(ii, other.get_unchecked_ref(ii));
  } // ... iadd(...)

  /**
   *  \brief  Subtracts two vectors.
   *  \param  other   The subtrahend.
   *  \param  result  The vectror to write the difference between this and other to.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual void sub(const derived_type& other, derived_type& result) const
  {
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    if (result.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of result (" << result.size() << ") does not match the size of this (" << size() << ")!");
    for (size_t ii = 0; ii < size(); ++ii)
      result.set_entry(ii, get_unchecked_ref(ii) - other.get_unchecked_ref(ii));
  } // ... sub(...)

  /**
   *  \brief  Subtracts two vectors.
   *  \param  other The subtrahend.
   *  \return The difference between this and other.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual derived_type sub(const derived_type& other) const
  {
    derived_type result = this->copy();
    result.isub(other);
    return result;
  } // ... sub(...)

  /**
   *  \brief  Inplace variant of sub().
   *  \param  other The subtrahend.
   *  \note   If you override this method please use exceptions instead of assertions (for the python bindings).
   */
  virtual void isub(const derived_type& other)
  {
    static constexpr ScalarType neg_one(-1);
    if (other.size() != size())
      DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                 "The size of other (" << other.size() << ") does not match the size of this (" << size() << ")!");
    for (size_t ii = 0; ii < size(); ++ii)
      add_to_entry(ii, neg_one * other.get_unchecked_ref(ii));
  } // ... isub(...)

  using BaseType::operator*;

  /**
   *  \brief  Computes the scalar products between this and another vector.
   *  \param  other The second factor.
   *  \return The scalar product.
   *  \see dot()
   */
  virtual ScalarType operator*(const derived_type& other) const
  {
    return dot(other);
  }

  /**
   *  \brief  Adds another vector to this, in-place variant.
   *  \param  other The second summand.
   *  \return The sum of this and other.
   */
  virtual derived_type& operator+=(const derived_type& other)
  {
    iadd(other);
    return this->as_imp();
  }

  template <class T, class S>
  derived_type& operator+=(const VectorInterface<T, S>& other)
  {
    for (size_t ii = 0; ii < size(); ++ii)
      add_to_entry(ii, other.get_entry(ii));
    return this->as_imp();
  }

  /**
   *  \brief  Subtracts another vector from this, in-place variant.
   *  \param  other The subtrahend.
   *  \return The difference between this and other.
   */
  virtual derived_type& operator-=(const derived_type& other)
  {
    isub(other);
    return this->as_imp();
  }

  template <class T, class S>
  derived_type& operator-=(const VectorInterface<T, S>& other)
  {
    for (size_t ii = 0; ii < size(); ++ii)
      add_to_entry(ii, -other.get_entry(ii));
    return this->as_imp();
  }

  /**
   *  \brief  Adds two vectors.
   *  \param  other The second summand.
   *  \return The sum of the two vectors.
   */
  virtual derived_type operator+(const derived_type& other) const
  {
    return add(other);
  }

  template <class T, class S>
  derived_type operator+(const VectorInterface<T, S>& other) const
  {
    return *this + as_derived(other);
  }

  /**
   *  \brief  Substracts two vectors.
   *  \param  other The subtrahend
   *  \return The difference.
   */
  virtual derived_type operator-(const derived_type& other) const
  {
    return sub(other);
  }

  template <class T, class S>
  derived_type operator-(const VectorInterface<T, S>& other) const
  {
    return *this - as_derived(other);
  }

  virtual derived_type& operator+=(const ScalarType& scalar)
  {
    for (auto& element : *this)
      element += scalar;
    return this->as_imp();
  }

  virtual derived_type& operator-=(const ScalarType& scalar)
  {
    for (auto& element : *this)
      element -= scalar;
    return this->as_imp();
  }

  derived_type& operator/=(const ScalarType& scalar) override
  {
    for (auto& element : *this)
      element /= scalar;
    return this->as_imp();
  }

  /**
   *  \brief  Check vectors for equality (componentwise) using almost_equal()
   *  \param  other   A vector of same size to compare with.
   *  \return Truth value of the comparison.
   *  \see    almost_equal()
   */
  virtual bool operator==(const derived_type& other) const
  {
    return almost_equal(other);
  }

  /**
   *  \brief  Check vectors for inequality using !almost_equal()
   *  \param  other   A vector of same size to compare with.
   *  \return Truth value of the comparison.
   */
  virtual bool operator!=(const derived_type& other) const
  {
    return !(this->operator==(other));
  }

  iterator begin()
  {
    return iterator(*this);
  }

  const_iterator begin() const
  {
    return const_iterator(*this);
  }

  iterator end()
  {
    return iterator(*this, true);
  }

  const_iterator end() const
  {
    return const_iterator(*this, true);
  }

  operator std::vector<ScalarType>() const
  {
    std::vector<ScalarType> ret(this->size());
    for (size_t ii = 0; ii < this->size(); ++ii)
      ret[ii] = this->operator[](ii);
    return ret;
  }

protected:
  template <class T, class S>
  derived_type as_derived(const VectorInterface<T, S>& other) const
  {
    assert(other.size() == size());
    derived_type ret(size());
    for (size_t ii = 0; ii < other.size(); ++ii)
      ret.set_entry(ii, other.get_entry(ii));
    return ret;
  }

  struct complex_switch
  {
    static ScalarType min(const derived_type& self)
    {
      if constexpr (!Common::is_complex<ScalarType>::value) {
        using std::min;
        ScalarType ret = 0;
        for (const auto& element : self)
          ret = min(ret, element);
        return ret;
      } else {
        DUNE_THROW(Exceptions::not_available, "For complex data types (implement this if you think otherwise)!");
      }
    }

    static ScalarType max(const derived_type& self)
    {
      if constexpr (!Common::is_complex<ScalarType>::value) {
        using std::max;
        ScalarType ret = 0;
        for (const auto& element : self)
          ret = max(ret, element);
        return ret;
      } else {
        DUNE_THROW(Exceptions::not_available, "For complex data types (implement this if you think otherwise)!");
      }
    }

    template <class T>
    static ScalarType dot(const derived_type& self, const VectorInterface<T, ScalarType>& other)
    {
      if constexpr (!Common::is_complex<ScalarType>::value) {
        if (other.size() != self.size())
          DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                     "The size of other (" << other.size() << ") does not match the size of this (" << self.size()
                                           << ")!");
        ScalarType result = 0;
        for (size_t ii = 0; ii < self.size(); ++ii)
          result += self.get_unchecked_ref(ii) * other.get_entry(ii);
        return result;
      } else {
        using std::conj;
        if (other.size() != self.size())
          DUNE_THROW(Common::Exceptions::shapes_do_not_match,
                     "The size of other (" << other.size() << ") does not match the size of this (" << self.size()
                                           << ")!");
        ScalarType result = 0;
        for (size_t ii = 0; ii < self.size(); ++ii)
          result += conj(self.get_unchecked_ref(ii)) * other.get_entry(ii);
        return result;
      }
    }
  }; // struct complex_switch
#ifndef __clang_analyzer__ // tidy throws away the second line
  template <class T, class S>
  friend std::ostream& operator<<(std::ostream& /*out*/, const VectorInterface<T, S>& /*vector*/);
#endif // __clang_analyzer__
}; // class VectorInterface


namespace internal {


template <class VectorImp>
struct VectorAbstractionBase
{
  static constexpr bool is_vector = LA::is_vector<VectorImp>::value;

  static constexpr bool has_static_size = false;

  static constexpr size_t static_size = std::numeric_limits<size_t>::max();

  static constexpr bool is_contiguous = true;

  using VectorType = VectorImp;
  using ScalarType = typename VectorImp::ScalarType;
  using RealType = typename VectorImp::RealType;
  using S = ScalarType;
  using R = RealType;

  template <size_t SIZE = static_size, class Field = ScalarType>
  using VectorTypeTemplate = typename std::conditional<is_vector, VectorImp, void>::type;

  template <size_t SIZE = static_size>
  static inline typename std::enable_if<is_vector, VectorType>::type
  create(const size_t sz, const ScalarType& val = Common::suitable_default<ScalarType>::value())
  {
    return VectorType(sz, val);
  }

  static inline ScalarType get_entry(const VectorType& vector, const size_t ii)
  {
    return vector.get_entry(ii);
  }

  static inline void set_entry(VectorType& vector, const size_t ii, const ScalarType& val)
  {
    vector.set_entry(ii, val);
  }

  static inline void add_to_entry(VectorType& vector, const size_t ii, const ScalarType& val)
  {
    vector.add_to_entry(ii, val);
  }

  static inline ScalarType* data(VectorType& vec)
  {
    return &(vec[0]);
  }

  static inline const ScalarType* data(const VectorType& vec)
  {
    return &(vec[0]);
  }
}; // struct VectorAbstractionBase


} // namespace internal


template <class T, class S>
std::ostream& operator<<(std::ostream& out, const VectorInterface<T, S>& vector)
{
  out << "[";
  const size_t sz = vector.size();
  if (sz > 0) {
    out << vector[0];
    for (size_t ii = 1; ii < sz; ++ii)
      out << " " << vector[ii];
  }
  out << "]";
  return out;
} // ... operator<<(...)


} // namespace Dune::XT::LA

#endif // DUNE_XT_LA_CONTAINER_VECTOR_INTERFACE_HH
