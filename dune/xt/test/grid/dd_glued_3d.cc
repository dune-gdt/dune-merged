// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017 - 2018)
//   René Fritze     (2018 - 2020)
//   Tobias Leibner  (2017 - 2020)

#include <dune/xt/test/main.hxx> // <- this one has to come first (includes the config.h)!

#include "dd_glued.hh"

#if HAVE_DUNE_GRID_GLUE

namespace Dune::XT::Grid {


template <>
struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>,
                       YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>>
{
  static int num_coarse_refinements()
  {
    return 0;
  }

  static int num_local_refinements()
  {
    return 2;
  }

  static std::string id()
  {
    return "3d_yaspgrid_yaspgrid";
  }

  static std::set<size_t> num_local_couplings_intersections()
  {
    // we expect 16 rectangles, each containing two triangles
    return {32};
  }
}; // struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, YaspGrid<3,
// EquidistantOffsetCoordinates<double, 3>>, anything>

#  if HAVE_DUNE_ALUGRID

template <class Comm>
struct ExpectedResults<ALUGrid<3, 3, cube, nonconforming, Comm>, YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>>
{
  static int num_coarse_refinements()
  {
    return 0;
  }

  static int num_local_refinements()
  {
    return 2;
  }

  static std::string id()
  {
    return "3d_alucubenonconforminggrid_yaspgrid";
  }

  static std::set<size_t> num_local_couplings_intersections()
  {
    return {32};
  }
}; // struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, ALUGrid<3, 3, simplex, nonconforming,
// Comm>, anything>

template <class Comm>
struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, ALUGrid<3, 3, cube, nonconforming, Comm>>
{
  static int num_coarse_refinements()
  {
    return 0;
  }

  static int num_local_refinements()
  {
    return 2;
  }

  static std::string id()
  {
    return "3d_yaspgrid_alucubenonconforminggrid";
  }

  static std::set<size_t> num_local_couplings_intersections()
  {
    return {32};
  }
}; // ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, ALUGrid<3, 3, cube, nonconforming, Comm>,
// anything>

#  endif // HAVE_DUNE_ALUGRID
#  if HAVE_DUNE_UGGRID || HAVE_UG

template <>
struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, UGGrid<3>>
{
  static int num_coarse_refinements()
  {
    return 0;
  }

  static int num_local_refinements()
  {
    return 2;
  }

  static std::string id()
  {
    return "3d_yaspgrid_uggrid";
  }

  static std::set<size_t> num_local_couplings_intersections()
  {
    // we expect 16 rectangles, each containing two triangles
    return {32};
  }
}; // struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, UGGrid<3>, anything>

#  endif // HAVE_DUNE_UGGRID || HAVE_UG
#  if HAVE_ALBERTA

template <>
struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, AlbertaGrid<3, 3>>
{
  static int num_coarse_refinements()
  {
    return 0;
  }

  static int num_local_refinements()
  {
    return 2;
  }

  static std::string id()
  {
    return "3d_yaspgrid_albertagrid";
  }

  static std::set<size_t> num_local_couplings_intersections()
  {
    return {10};
  }
}; // struct ExpectedResults<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, AlbertaGrid<3>, anything>

#  endif // HAVE_ALBERTA

} // namespace Dune::XT::Grid


using namespace Dune;
using namespace Dune::XT::Grid;


// clang-format off
using GridTypes = ::testing::Types< std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>,
                                     YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>>
#if HAVE_DUNE_ALUGRID
                        , std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>,
                                     Dune::ALUGrid<3, 3, cube, nonconforming>>
//                      , std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>,
//                                   ALUGrid<3, 3, simplex, conforming>>                  // <- known to fail completely
//                      , std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>,
//                                   ALUGrid<3, 3, simplex, nonconforming>>               // <- known to fail completely
                        , std::tuple<Dune::ALUGrid<3, 3, cube, nonconforming>,
                                     YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>>
//                      , std::tuple<ALUGrid<3, 3, simplex, conforming>,
//                                   ALUGrid<3, 3, simplex, nonconforming>>               // <- known to fail completely
//                      , std::tuple<ALUGrid<3, 3, simplex, nonconforming>,
//                                   ALUGrid<3, 3, simplex, nonconforming>>               // <- known to fail completely
//                      , std::tuple<ALUGrid<3, 3, cube, nonconforming>,
//                                   ALUGrid<3, 3, simplex, nonconforming>>               // <- known to fail completely
#endif // HAVE_DUNE_ALUGRID
#if !HAVE_MPI && (HAVE_DUNE_UGGRID || HAVE_UG)
                        , std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, UGGrid<3>>
#endif
#if HAVE_ALBERTA
                        , std::tuple<YaspGrid<3, EquidistantOffsetCoordinates<double, 3>>, AlbertaGrid<3, 3>>
#endif
                        >; // clang-format on

TYPED_TEST_SUITE(GluedDdGridTest, GridTypes);
TYPED_TEST(GluedDdGridTest, setup_works)
{
  this->setup();
}
TYPED_TEST(GluedDdGridTest, visualize_is_callable)
{
  this->visualize_is_callable();
}
TYPED_TEST(GluedDdGridTest, couplings_are_of_correct_size)
{
  this->couplings_are_of_correct_size();
}


#endif // HAVE_DUNE_GRID_GLUE
