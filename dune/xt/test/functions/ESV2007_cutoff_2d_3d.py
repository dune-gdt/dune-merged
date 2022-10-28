# ~~~
# This file is part of the dune-gdt project:
#   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-gdt
# Copyright 2010-2021 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   René Fritze    (2018 - 2019)
#   Tim Keil       (2018)
#   Tobias Leibner (2019 - 2021)
#
# This file is part of the dune-xt project:
# ~~~

import grids
import itertools
from dune.xt.codegen import typeid_to_typedef_name

dimDomain = [2, 3]

multi_out = {grids.pretty_print(g[0], g[1]): g[0] for g in grids.type_and_dim(cache, dimDomain)}

multi_out = {filename + '.cc': {'types': [(filename, grid)]} for filename, grid in multi_out.items()}
