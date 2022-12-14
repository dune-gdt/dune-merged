# ~~~
# This file is part of the dune-xt project:
#   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
# Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   Felix Schindler (2017)
#   René Fritze     (2016 - 2020)
#   Tobias Leibner  (2018, 2020 - 2021)
# ~~~

macro(ADD_IF_SUPPORTED dest)
  foreach(flag ${ARGN})
    check_cxx_accepts_flag("${flag}" has_${flag})
    if(has_${flag})
      set(${dest} "${${dest}} ${flag}")
    else(has_${flag})
      message("compiler doesn't support: ${flag}")
    endif(has_${flag})
  endforeach(flag ${ARGN})
endmacro(ADD_IF_SUPPORTED)

macro(INCLUDE_SYS_DIR)
  foreach(arg ${ARGN})
    if(IS_DIRECTORY ${arg})
      # due to https://gcc.gnu.org/bugzilla/show_bug.cgi?id=70129  we have to filter what to sys-include includes
      include_directories(SYSTEM ${arg})
    else(IS_DIRECTORY ${arg})
      message(STATUS "Include directory ${arg} does not exist")
    endif(IS_DIRECTORY ${arg})
  endforeach(arg)
endmacro(INCLUDE_SYS_DIR)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "
    #include <map>
    int main(void)
    {
      std::map<int, int> a;
      a.emplace(2, 2);
      return 0;
    };
"
  HAVE_MAP_EMPLACE)

check_cxx_source_compiles(
  "
    void foo([[maybe_unused]] bool arg) {}
    int main(void){};
"
  HAS_WORKING_UNUSED_ATTRIBUTE)
