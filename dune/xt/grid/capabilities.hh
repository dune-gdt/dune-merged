// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   René Fritze     (2018 - 2020)
//   Tobias Leibner  (2018 - 2020)

#ifndef DUNE_XT_GRID_CAPABILITIES_HH
#define DUNE_XT_GRID_CAPABILITIES_HH

#include <dune/common/version.hh>

#include "grids.hh"

namespace Dune::XT::Grid {


template <class G>
struct has_boundary_id
{
#if DUNE_VERSION_GTE(DUNE_GRID, 2, 6)
  static constexpr bool value = false;
#else
#  if defined(DUNE_GRID_EXPERIMENTAL_GRID_EXTENSIONS) && DUNE_GRID_EXPERIMENTAL_GRID_EXTENSIONS
  static constexpr bool value = true;
#  else
  static constexpr bool value = false;
#  endif
#endif
};


#if HAVE_DUNE_UGGRID || HAVE_UG

template <int dim>
struct has_boundary_id<UGGrid<dim>>
{
  static constexpr bool value = false;
};

#endif // HAVE_DUNE_UGGRID || HAVE_UG


} // namespace Dune::XT::Grid

#endif // DUNE_XT_GRID_CAPABILITIES_HH
