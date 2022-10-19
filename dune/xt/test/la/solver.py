# ~~~
# This file is part of the dune-gdt project:
#   https://zivgitlab.uni-muenster.de/ag-ohlberger/dune-community/dune-gdt
# Copyright 2010-2021 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   Felix Schindler (2016)
#   René Fritze     (2017 - 2019)
#   Tobias Leibner  (2015 - 2017, 2019 - 2021)
#
# This file is part of the dune-xt project:
# ~~~

from matrices import commontype, latype
from dune.xt.codegen import have_eigen, have_istl, typeid_to_typedef_name as safe_name

types = [
    f.split('_') for f in [
        'CommonDenseMatrix_CommonDenseVector_CommonDenseVector_complex',
        'CommonDenseMatrix_CommonDenseVector_CommonDenseVector_double',
        'EigenDenseMatrix_EigenDenseVector_EigenDenseVector_complex',
        'EigenDenseMatrix_EigenDenseVector_EigenDenseVector_double',
        'EigenDenseMatrix_EigenDenseVector_EigenMappedDenseVector_double',
        'EigenDenseMatrix_EigenMappedDenseVector_EigenDenseVector_double',
        'EigenDenseMatrix_EigenMappedDenseVector_EigenMappedDenseVector_double',
        'EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_complex',
        'EigenRowMajorSparseMatrix_EigenDenseVector_EigenDenseVector_double',
        'FieldMatrix_FieldVector_FieldVector_complex', 'FieldMatrix_FieldVector_FieldVector_double',
        'IstlRowMajorSparseMatrix_IstlDenseVector_IstlDenseVector_double'
    ]
]


def test_tuple(args):
    o, r, s, f = args
    if f == 'complex':
        f = 'std::complex<double>'
    if o == 'FieldMatrix':
        fm = '{}, 10, 10'.format(f)
        fv = '{}, 10'.format(f)
        return (safe_name('{}_{}_{}_{}'.format(o, r, s, f)), commontype(o, fm), commontype(r, fv), commontype(s, fv))
    else:
        return (safe_name('{}_{}_{}_{}'.format(o, r, s, f)), latype(o, f), latype(r, f), latype(s, f))


def type_ok(t):
    if sum(['Eigen' in x for x in t]):
        return have_eigen(cache)
    if sum(['Istl' in x for x in t]):
        return have_istl(cache)
    return True


testtypes = [test_tuple(item) for item in types if type_ok(item)]
