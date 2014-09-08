// This file is part of the dune-stuff project:
//   https://github.com/wwu-numerik/dune-stuff
// Copyright holders: Rene Milk, Felix Schindler
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

#ifndef DUNE_STUFF_MEMORY_HH
#define DUNE_STUFF_MEMORY_HH

#include <memory>

namespace Dune {
namespace Stuff {
namespace Common {


//! make_unique implementation via herb sutter: http://herbsutter.com/gotw/_102/
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


namespace internal {


template <class T>
class ConstAccessInterface
{
public:
  virtual ~ConstAccessInterface()
  {
  }

  virtual const T& access() const = 0;
}; // class ConstAccessInterface


template <class T>
class ConstAccessByReference : public ConstAccessInterface<T>
{
public:
  ConstAccessByReference(const T& tt)
    : tt_(tt)
  {
  }

  ConstAccessByReference(const ConstAccessByReference<T>& other) = delete;

  ConstAccessByReference(ConstAccessByReference<T>&& source) = delete;

  virtual ~ConstAccessByReference()
  {
  }

  ConstAccessByReference<T>& operator=(const ConstAccessByReference<T>& other) = delete;

  ConstAccessByReference<T>& operator=(ConstAccessByReference<T>&& source) = delete;

  virtual const T& access() const
  {
    return tt_;
  }

private:
  const T& tt_;
}; // class ConstAccessByReference


template <class T>
class ConstAccessByPointer : public ConstAccessInterface<T>
{
public:
  ConstAccessByPointer(const T* tt)
    : tt_(tt)
  {
  }

  ConstAccessByPointer(std::unique_ptr<const T>&& tt)
    : tt_(tt)
  {
  }

  ConstAccessByPointer(std::shared_ptr<const T> tt)
    : tt_(tt)
  {
  }

  ConstAccessByPointer(const ConstAccessByPointer<T>& other) = delete;

  ConstAccessByPointer(ConstAccessByPointer<T>&& source) = delete;

  virtual ~ConstAccessByPointer()
  {
  }

  ConstAccessByPointer<T>& operator=(const ConstAccessByPointer<T>& other) = delete;

  ConstAccessByPointer<T>& operator=(ConstAccessByPointer<T>&& source) = delete;

  virtual const T& access() const
  {
    return *tt_;
  }

private:
  const std::shared_ptr<const T> tt_;
}; // class ConstAccessByPointer


template <class T>
class AccessInterface
{
public:
  virtual ~AccessInterface()
  {
  }

  virtual T& access() = 0;

  virtual const T& access() const = 0;
}; // class AccessInterface


template <class T>
class AccessByReference : public AccessInterface<T>
{
public:
  AccessByReference(T& tt)
    : tt_(tt)
  {
  }

  AccessByReference(const AccessByReference<T>& other) = delete;

  AccessByReference(AccessByReference<T>&& source) = delete;

  virtual ~AccessByReference()
  {
  }

  AccessByReference<T>& operator=(const AccessByReference<T>& other) = delete;

  AccessByReference<T>& operator=(AccessByReference<T>&& source) = delete;

  virtual T& access()
  {
    return tt_;
  }

  virtual const T& access() const
  {
    return tt_;
  }

private:
  T& tt_;
}; // class AccessByReference


template <class T>
class AccessByPointer : public AccessInterface<T>
{
public:
  AccessByPointer(T* tt)
    : tt_(tt)
  {
  }

  AccessByPointer(std::unique_ptr<T>&& tt)
    : tt_(tt)
  {
  }

  AccessByPointer(std::shared_ptr<T> tt)
    : tt_(tt)
  {
  }

  AccessByPointer(const AccessByPointer<T>& other) = delete;

  AccessByPointer(AccessByPointer<T>&& source) = delete;

  virtual ~AccessByPointer()
  {
  }

  AccessByPointer<T>& operator=(const AccessByPointer<T>& other) = delete;

  AccessByPointer<T>& operator=(AccessByPointer<T>&& source) = delete;

  virtual T& access()
  {
    return *tt_;
  }

  virtual const T& access() const
  {
    return *tt_;
  }

private:
  std::shared_ptr<T> tt_;
}; // class AccessByPointer


} // namespace internal


template <class T>
class ConstStorageProvider
{
public:
  ConstStorageProvider(const T& tt)
    : provide_(make_unique<internal::ConstAccessByReference<T>>(tt))
  {
  }

  ConstStorageProvider(const T* tt)
    : provide_(make_unique<internal::ConstAccessByPointer<T>>(tt))
  {
  }

  ConstStorageProvider(std::unique_ptr<const T>&& tt)
    : provide_(make_unique<internal::ConstAccessByPointer<T>>(tt))
  {
  }

  ConstStorageProvider(std::shared_ptr<const T> tt)
    : provide_(make_unique<internal::ConstAccessByPointer<T>>(tt))
  {
  }

  /**
   * Since the intended behaviour is unknown, call ConstStorageProvider(new T(tt)) or ConstStorageProvider(tt) manually!
   */
  ConstStorageProvider(const ConstStorageProvider<T>& other) = delete;

  ConstStorageProvider(ConstStorageProvider<T>&& source) = delete;

  ConstStorageProvider<T> operator=(const ConstStorageProvider<T>& other) = delete;

  ConstStorageProvider<T> operator=(ConstStorageProvider<T>&& source) = delete;

  const T& storage_access() const
  {
    return provide_->access();
  }

private:
  std::unique_ptr<internal::ConstAccessInterface<T>> provide_;
}; // class ConstStorageProvider


template <class T>
class StorageProvider
{
public:
  StorageProvider(T& tt)
    : provide_(make_unique<internal::AccessByReference<T>>(tt))
  {
  }

  StorageProvider(T* tt)
    : provide_(make_unique<internal::AccessByPointer<T>>(tt))
  {
  }

  StorageProvider(std::unique_ptr<T>&& tt)
    : provide_(make_unique<internal::AccessByPointer<T>>(tt))
  {
  }

  StorageProvider(std::shared_ptr<T> tt)
    : provide_(make_unique<internal::AccessByPointer<T>>(tt))
  {
  }

  /**
   * Since the intended behaviour is unknown, call StorageProvider(new T(tt)) or StorageProvider(tt) manually!
   */
  StorageProvider(const StorageProvider<T>& other) = delete;

  StorageProvider(StorageProvider<T>&& source) = delete;

  StorageProvider<T> operator=(const StorageProvider<T>& other) = delete;

  StorageProvider<T> operator=(StorageProvider<T>&& source) = delete;

  T& storage_access()
  {
    return provide_->access();
  }

  const T& storage_access() const
  {
    return provide_->access();
  }

private:
  std::unique_ptr<internal::AccessInterface<T>> provide_;
}; // class StorageProvider


} // namespace Common
} // namespace Stuff
} // namespace Dune

#endif // DUNE_STUFF_MEMORY_HH
