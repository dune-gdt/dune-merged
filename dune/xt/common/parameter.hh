// This file is part of the dune-xt-common project:
//   https://github.com/dune-community/dune-xt-common
// Copyright 2009-2017 dune-xt-common developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)

#ifndef DUNE_XT_COMMON_PARAMETER_HH
#define DUNE_XT_COMMON_PARAMETER_HH

#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <sstream>
#include <ostream>

namespace Dune {
namespace XT {
namespace Common {
namespace internal {


template <class ValueType>
class SimpleDict
{
public:
  SimpleDict();

  SimpleDict(const std::string& key, const ValueType& value);

  SimpleDict(const std::vector<std::pair<std::string, ValueType>>& key_value_pairs);

  const std::vector<std::string>& keys() const;

  bool empty() const;

  bool has_key(const std::string& key) const;

  void set(const std::string& key, const ValueType& value, const bool overwrite = false);

  const ValueType& get(const std::string& key) const;

  bool operator<(const SimpleDict<ValueType>& other) const;

  bool operator==(const SimpleDict<ValueType>& other) const;

  bool operator!=(const SimpleDict<ValueType>& other) const;

  size_t size() const;

protected:
  std::string report(const std::string& prefix) const;

  void update_keys();

  std::map<std::string, ValueType> dict_;
  std::vector<std::string> keys_;
}; // class SimpleDict


extern template class SimpleDict<size_t>;
extern template class SimpleDict<std::vector<double>>;


} // namespace internal


class ParameterType : public internal::SimpleDict<size_t>
{
  typedef internal::SimpleDict<size_t> BaseType;

public:
  ParameterType();

  ParameterType(const std::string& key);

  ParameterType(const std::string& key, const size_t& sz);

  ParameterType(const std::pair<std::string, size_t>& key_size_pair);

  ParameterType(const std::pair<const char*, int>& key_size_pair);

  ParameterType(const std::vector<std::pair<std::string, size_t>>& key_size_pairs);

  std::string report() const;
}; // class ParameterType


std::ostream& operator<<(std::ostream& out, const ParameterType& param_type);


class Parameter : public internal::SimpleDict<std::vector<double>>
{
  typedef SimpleDict<std::vector<double>> BaseType;
  typedef std::vector<double> ValueType;

public:
  Parameter(const std::vector<std::pair<std::string, ValueType>>& key_value_pairs = {});

  Parameter(const double& value);

  Parameter(const std::string& key, const double& value);

  Parameter(const std::string& key, const ValueType& value);

  ParameterType type() const;

  std::string report() const;
}; // class Parameter


std::ostream& operator<<(std::ostream& out, const Parameter& mu);


class ParametricInterface
{
public:
  virtual ~ParametricInterface() = default;

  virtual bool is_parametric() const;

  virtual const ParameterType& parameter_type() const;

  Parameter parse_parameter(const Parameter& mu) const;

private:
  ParameterType none_parameter_type_;
}; // class ParametricInterface


} // namespace Common
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_COMMON_PARAMETER_HH