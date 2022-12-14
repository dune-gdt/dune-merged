// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2018)
//   René Fritze     (2016, 2018 - 2020)
//   Tobias Leibner  (2016 - 2020)

#ifndef DUNE_XT_GRID_BOUNDARYINFO_TYPES_HH
#define DUNE_XT_GRID_BOUNDARYINFO_TYPES_HH

#include <dune/xt/grid/exceptions.hh>
#include "interfaces.hh"

namespace Dune::XT::Grid {

// We do not want to add a virtual destructor (to be able to use this as constexpr), so just silence the warning.
#if (defined(BOOST_CLANG) && BOOST_CLANG) || (defined(BOOST_GCC) && BOOST_GCC)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#  pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif


class NoBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "no_boundary";
  }

  BoundaryType* copy() const final
  {
    return new NoBoundary();
  }
};


class UnknownBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "unknown_boundary";
  }

  BoundaryType* copy() const final
  {
    return new UnknownBoundary();
  }
};


class DirichletBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "dirichlet_boundary";
  }

  BoundaryType* copy() const final
  {
    return new DirichletBoundary();
  }
};


class NeumannBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "neumann_boundary";
  }

  BoundaryType* copy() const final
  {
    return new NeumannBoundary();
  }
};


class RobinBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "robin_boundary";
  }

  BoundaryType* copy() const final
  {
    return new RobinBoundary();
  }
};


class ReflectingBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "reflecting_boundary";
  }

  BoundaryType* copy() const final
  {
    return new ReflectingBoundary();
  }
};


class AbsorbingBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "absorbing_boundary";
  }

  BoundaryType* copy() const final
  {
    return new AbsorbingBoundary();
  }
};


class InflowBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "inflow_boundary";
  }

  BoundaryType* copy() const final
  {
    return new InflowBoundary();
  }
};


class OutflowBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "outflow_boundary";
  }

  BoundaryType* copy() const final
  {
    return new OutflowBoundary();
  }
};


class InflowOutflowBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "inflow_outflow_boundary";
  }

  BoundaryType* copy() const final
  {
    return new InflowOutflowBoundary();
  }
};


class ImpermeableBoundary : public BoundaryType
{
public:
  std::string id() const final
  {
    return "impermeable_boundary";
  }

  BoundaryType* copy() const final
  {
    return new ImpermeableBoundary();
  }
};


#if (defined(BOOST_CLANG) && BOOST_CLANG) || (defined(BOOST_GCC) && BOOST_GCC)
#  pragma GCC diagnostic pop
#endif


// NOLINTNEXTLINE(clang-diagnostic-unused-function)
static inline BoundaryType* make_boundary_type(const std::string& id)
{
  if (id == NoBoundary().id())
    return new NoBoundary();
  if (id == UnknownBoundary().id())
    return new UnknownBoundary();
  if (id == DirichletBoundary().id())
    return new DirichletBoundary();
  if (id == NeumannBoundary().id())
    return new NeumannBoundary();
  if (id == RobinBoundary().id())
    return new RobinBoundary();
  if (id == ReflectingBoundary().id())
    return new ReflectingBoundary();
  if (id == InflowBoundary().id())
    return new InflowBoundary();
  if (id == OutflowBoundary().id())
    return new OutflowBoundary();
  if (id == InflowBoundary().id())
    return new InflowBoundary();
  if (id == InflowOutflowBoundary().id())
    return new InflowOutflowBoundary();
  if (id == ImpermeableBoundary().id())
    return new ImpermeableBoundary();
  DUNE_THROW(Exceptions::boundary_type_error, "id: " << id);
  return new UnknownBoundary();

} // ... make_boundary_type(...)


static constexpr AbsorbingBoundary absorbing_boundary{};
static constexpr DirichletBoundary dirichlet_boundary{};
static constexpr NeumannBoundary neumann_boundary{};
static constexpr NoBoundary no_boundary{};
static constexpr ReflectingBoundary reflecting_boundary{};
static constexpr RobinBoundary robin_boundary{};
static constexpr UnknownBoundary unknown_boundary{};


} // namespace Dune::XT::Grid

#endif // DUNE_XT_GRID_BOUNDARYINFO_TYPES_HH
