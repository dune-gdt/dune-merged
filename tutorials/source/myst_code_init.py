# ~~~
# This file is part of the dune-gdt project:
#   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-gdt
# Copyright 2010-2021 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
# ~~~

from IPython import get_ipython
import warnings

ip = get_ipython()
if ip is not None:
    # ip.run_line_magic('load_ext', 'pymor.discretizers.builtin.gui.jupyter')
    ip.run_line_magic("matplotlib", "inline")

warnings.filterwarnings("ignore", category=UserWarning, module="torch")
