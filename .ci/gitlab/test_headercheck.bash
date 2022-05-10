#!/bin/bash
#
# ~~~
# This file is part of the dune-xt project:
#   https://github.com/dune-community/dune-xt
# Copyright 2009-2020 dune-xt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   René Fritze    (2019)
#   Tobias Leibner (2019 - 2020)
# ~~~

set -ex

OPTS_PATH=./deps/config.opts/${OPTS}
source ${OPTS_PATH}
set -u

DUNECONTROL=dunecontrol
BUILD_CMD="ninja -v -j2"

${DUNECONTROL} --opts=${OPTS_PATH} --only=${MY_MODULE} all
${DUNECONTROL} --opts=${OPTS_PATH} --only=${MY_MODULE} bexec ${BUILD_CMD}
if [ "${TESTS_MODULE_SUBDIR}" = "gdt" ] ; then
  HEADERCHECK="headercheck"
else
  HEADERCHECK="${TESTS_MODULE_SUBDIR}_headercheck"
fi

${DUNECONTROL} --opts=${OPTS_PATH} --only=${MY_MODULE} bexec ${BUILD_CMD} ${HEADERCHECK}
