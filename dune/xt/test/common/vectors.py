# ~~~
# This file is part of the dune-gdt project:
#   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-gdt
# Copyright 2010-2021 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   René Fritze    (2017 - 2019)
#   Tobias Leibner (2018 - 2021)
#
# This file is part of the dune-xt project:
# ~~~

from dune.xt import codegen


def vectors(cache):
    vecs = ['FieldVector', 'DynamicVector', 'CommonDenseVector', 'CommonSparseVector']
    if codegen.have_eigen(cache):
        vecs.extend(['EigenDenseVector', 'EigenMappedDenseVector'])
    if codegen.have_istl(cache):
        vecs.append('IstlDenseVector')
    return vecs


def vectortype(c, f):
    if c == 'FieldVector':
        f = '{}, 4'.format(f)
        return commontype(c, f)
    elif c == 'DynamicVector':
        return dunetype(c, f)
    else:
        return latype(c, f)


def latype(c, f):
    return 'Dune::XT::LA::{}<{}>'.format(c, f)


def commontype(c, f):
    return 'Dune::XT::Common::{}<{}>'.format(c, f)


def dunetype(c, f):
    return 'Dune::{}<{}>'.format(c, f)


def fieldtypes(cache):
    return ['double', 'std::complex<double>']


def vector_filter(vector, field):
    return not ('EigenMappedDenseVector' in vector and 'complex' in field)
