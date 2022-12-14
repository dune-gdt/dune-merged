// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2020)

#include "config.h"

#include "traits.hh"


PYBIND11_MODULE(_common_traits, m)
{
  namespace py = pybind11;
  using namespace Dune::XT::Common::bindings;

#define BIND_(NAME)                                                                                                    \
  pybind11::class_<NAME>(m, #NAME, (std::string("tag: ") + #NAME).c_str())                                             \
      .def(py::init())                                                                                                 \
      .def("__repr__", [](const NAME&) { return std::string(#NAME) + "()"; })

  BIND_(Double);

#undef BIND_
}
