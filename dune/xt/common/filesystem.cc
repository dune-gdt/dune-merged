// This file is part of the dune-xt project:
//   https://github.com/dune-community/dune-xt
// Copyright 2009-2020 dune-xt developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2012, 2014, 2016 - 2017)
//   René Fritze     (2010 - 2016, 2018 - 2019)
//   Sven Kaulmann   (2011)
//   Tobias Leibner  (2020)

#include "config.h"

#include "filesystem.hh"

namespace Dune {
namespace XT {
namespace Common {


//! strip filename from \path if present, return empty string if only filename present
std::string directory_only(std::string _path)
{
  return std::filesystem::path(_path).parent_path().string();
}

//! return everything after the last slash
std::string filename_only(const std::string& _path)
{
  return std::filesystem::path(_path).filename().string();
} // filename_only

//! may include filename, will be stripped
void test_create_directory(const std::string _path)
{
  std::string pathonly = directory_only(_path);
  if (!pathonly.empty())
    std::filesystem::create_directories(pathonly);
}

//! pure c++ emulation of system's touch binary
bool touch(const std::string& _path)
{
  return std::ofstream(_path.c_str()).is_open();
}

std::unique_ptr<std::ofstream> make_ofstream(const std::filesystem::path& path, const std::ios_base::openmode mode)
{
  test_create_directory(path.string());
  return std::make_unique<std::ofstream>(path, mode);
}

std::unique_ptr<std::ifstream> make_ifstream(const std::filesystem::path& path, const std::ios_base::openmode mode)
{
  test_create_directory(path.string());
  return std::make_unique<std::ifstream>(path, mode);
}

//! read a file and output all lines containing filter string to a stream
void file_to_stream_filtered(std::ostream& stream, std::string filename, std::string filter)
{
  std::ifstream file(filename.c_str(), std::ifstream::in);
  std::string line;
  while (file.good()) {
    std::getline(file, line);
    if (line.find(filter) != std::string::npos)
      stream << line << "\n";
  }
  file.close();
} // file_to_stream_filtered

//! output programs mem usage stats by reading from /proc
void meminfo(LogStream& stream)
{
  stream << "Memory info: \n";
  stream.resume();
  pid_t pid = getpid();
  std::stringstream filename;
  filename << "/proc/" << pid << "/status";

  file_to_stream_filtered(stream, filename.str(), "Vm");
  file_to_stream_filtered(stream, "/proc/meminfo", "Mem");
  stream << "------------ \n\n" << std::endl;
} // meminfo


} // namespace Common
} // namespace XT
} // namespace Dune
