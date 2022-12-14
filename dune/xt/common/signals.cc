// This file is part of the dune-xt project:
//   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-xt
// Copyright 2009-2021 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2014, 2016 - 2017)
//   René Fritze     (2013, 2015 - 2016, 2018 - 2020)
//   Tobias Leibner  (2018, 2020)

#include "config.h"

#include "signals.hh"

#include <dune/xt/common/logging.hh>
#include <dune/xt/common/string.hh>

namespace Dune::XT::Common {

//! reset given signal to default handler
void reset_signal(int signal)
{
  struct sigaction new_action;

  new_action.sa_handler = SIG_DFL;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;
  sigaction(signal, &new_action, nullptr);
} // reset_signal

//! example signal handler
void handle_interrupt(int signal)
{
  DXTC_LOG_INFO << "forcefully terminated at " << stringFromTime() << std::endl;
  // reset signal handler and commit suicide
  reset_signal(signal);
  kill(getpid(), signal);
} // handle_interrupt

//! type of handler functions
using handler_type = void(int);

//! calling this from your main() will install handler as callback when signal is received
void install_signal_handler(int signal, handler_type handler)
{
  struct sigaction new_action;

  /* Set up the structure to specify the new action. */
  new_action.sa_handler = handler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction(signal, &new_action, nullptr);
} // install_signal_handler

} // namespace Dune::XT::Common
