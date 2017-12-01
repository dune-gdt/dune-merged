# This file is part of the dune-xt-common project:
#   https://github.com/dune-community/dune-xt-common
# Copyright 2009-2017 dune-xt-common developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   Felix Schindler (2016 - 2017)
#   Rene Milk       (2016)
#   Tobias Leibner  (2016)

macro(add_analyze)
    find_program(ANALYZER clang-check)
    if(EXISTS ${ANALYZER})
        message(STATUS "adding analyze target")
        add_custom_target( analyze SOURCES ${ARGN} )
        foreach(_file ${ARGN})
            string(REPLACE "/" "_" fn ${_file})
            add_custom_target("analyze_${fn}" ${ANALYZER} -fixit -p=${CMAKE_CURRENT_BINARY_DIR} -analyze ${CMAKE_CURRENT_SOURCE_DIR}/${_file} )
            add_dependencies( analyze "analyze_${fn}" )
        endforeach(_file )
    else()
        message(WARNING "not adding analyze target because clang-check is missing")
    endif(EXISTS ${ANALYZER})
endmacro(add_analyze)

find_package(ClangFormat 3.9 EXACT)
macro(add_format glob_dir)
  if(${ARGC} GREATER 1)
    message(WARNING "'add_format' API has changed. Please provide a single "
                    "search directory instead of multiple filenames")
  endif()
  if(ClangFormat_FOUND)
    message(STATUS "adding format target")
    if (NOT TARGET format)
      add_custom_target( format  )
    endif (NOT TARGET format)
    string(REPLACE "/" "_" fn ${glob_dir})
    file(GLOB_RECURSE _files "${glob_dir}/*.hh" "${glob_dir}/*.cc"
        "${glob_dir}/*.cxx" "${glob_dir}/*.hxx" "${glob_dir}/*.h" "${glob_dir}/*.c" "${glob_dir}/*.pbh")
    add_custom_target("format_${fn}" ${ClangFormat_EXECUTABLE} -i -style=file -fallback-style=none ${_files} )
    add_dependencies( format "format_${fn}" )
  else()
    message(WARNING "not adding format target because clang-format is missing or "
                "wrong version: ${ClangFormat_EXECUTABLE} ${ClangFormat_VERSION}")
  endif(ClangFormat_FOUND)
endmacro(add_format)

find_package(ClangTidy 3.7 )
macro(add_tidy glob_dir)
  if(ClangTidy_FOUND)
    message(STATUS "adding tidy target")
    if (NOT TARGET tidy)
      add_custom_target( tidy )
    endif (NOT TARGET tidy)
    string(REPLACE "/" "_" fn ${glob_dir})
    file(GLOB_RECURSE _files "${glob_dir}/*.cc" "${glob_dir}/*.c")
    add_custom_target("tidy_${fn}" ${ClangTidy_EXECUTABLE} -p=${CMAKE_CURRENT_BINARY_DIR} ${_files} )
    add_dependencies( tidy "tidy_${fn}" )
  else()
    message(WARNING "not adding tidy target because clang-tidy is missing or"
                "wrong version: ${ClangTidy_EXECUTABLE} ${ClangTidy_VERSION}")
  endif(ClangTidy_FOUND)
endmacro(add_tidy)

macro(add_forced_doxygen_target)
  add_doxygen_target()
  if(TARGET doxygen_${ProjectName})
    add_custom_target(doxygen_${ProjectName}_pre_build COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/html )
    add_dependencies(doxygen_${ProjectName} doxygen_${ProjectName}_pre_build)
  endif()
endmacro(add_forced_doxygen_target)

MACRO( DEPENDENCYCHECK )
	ADD_CUSTOM_TARGET( dependencycheck SOURCES ${ARGN} )
	FOREACH( HEADER ${ARGN} )
		string(REPLACE "/" "_" fn ${HEADER})
		SET( TEST_NAME "dependencycheck_${fn}")
		TO_LIST_SPACES( CMAKE_CXX_FLAGS TEST_NAME_FLAGS )
		SET(XARGS ${TEST_NAME_FLAGS} -DHAVE_CONFIG_H -H -c ${HEADER} -w)
		ADD_CUSTOM_TARGET( ${TEST_NAME} + ${dune-xt-common_SOURCE_DIR}/cmake/dependencyinfo.py ${CMAKE_CXX_COMPILER} ${XARGS} ${CMAKE_CURRENT_SOURCE_DIR} ${fn}.dep)
		add_dependencies( dependencycheck ${TEST_NAME} )
	ENDFOREACH( HEADER )
ENDMACRO( DEPENDENCYCHECK )

MACRO( ADD_CPPCHECK )
	find_program( CPPCHECK_BINARY NAMES cppcheck )
	IF( EXISTS ${CPPCHECK_BINARY} )
		SET(CPPINLINST ${CMAKE_CURRENT_BINARY_DIR}/cppcheck.files )
		IF( EXISTS ${CPPINLINST} )
			file(REMOVE  ${CPPINLINST} )
		ENDIF()
		FOREACH( SOURCEFILE ${ARGN} )
			FILE( APPEND  ${CPPINLINST}  "${SOURCEFILE}\n" )
		ENDFOREACH( SOURCEFILE )
		TO_LIST_SPACES( CPPCHECK_FLAGS_SPLIT ${CMAKE_CXX_FLAGS} )
		ADD_CUSTOM_TARGET(  cppcheck cppcheck --enable=all --xml --report-progress --file-list=${CPPINLINST}
				${CPPCHECK_FLAGS_SPLIT}  2>cppcheck.xml )
	ELSE( EXISTS ${CPPCHECK_BINARY} )
		MESSAGE( STATUS "Not adding cppcheck target because cppcheck executable not found" )
	ENDIF( EXISTS ${CPPCHECK_BINARY} )
ENDMACRO( ADD_CPPCHECK )
