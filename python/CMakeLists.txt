# ~~~
# This file is part of the dune-gdt project:
# https://github.com/dune-community/dune-gdt
# Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
# or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
# with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
# René Fritze (2018)
# ~~~

include(DuneXTTesting)
include(DunePybindxiInstallPythonPackage)

# this symlinks all files in python/ to the binary dir and install into the virtualenv from there thereby making the
# compiled extensions directly usable from the venv
dune_pybindxi_install_python_package(PATH xt)
add_subdirectory(xt)
dune_pybindxi_install_python_package(PATH gdt)
add_subdirectory(gdt)

dxt_add_python_tests()
