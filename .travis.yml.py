#!/usr/bin/env python3
#
# This file is part of the dune-gdt project:
#   https://github.com/dune-community/dune-gdt
# Copyright 2010-2018 dune-gdt developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
#      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
#          with "runtime exception" (http://www.dune-project.org/license.html)
# Authors:
#   Felix Schindler (2016 - 2017)
#   Rene Milk       (2016 - 2017)

tpl = '''# ~~~
# This file is part of the dune-gdt project:
#   https://github.com/dune-community/dune-gdt
# Copyright 2010-2016 dune-gdt developers and contributors. All rights reserved.
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
# Authors:
#   Felix Schindler (2016 - 2017)
#   Rene Milk       (2016 - 2017)
# ~~~

# THIS FILE IS AUTOGENERATED -- DO NOT EDIT #


sudo: required
dist: trusty
language: generic
services: docker

before_script:
    - export IMAGE="dunecommunity/${MY_MODULE}-testing_${DOCKER_TAG}:${TRAVIS_BRANCH}"
    - ./.travis.add_swap.bash 2000 &
    # get image with fallback to master branch of the super repo
    - docker pull ${IMAGE} || export IMAGE="dunecommunity/${MY_MODULE}-testing_${DOCKER_TAG}:master" ; docker pull ${IMAGE}
    - docker inspect ${IMAGE}
    # for add swap
    - wait
    - export ENV_FILE=${HOME}/env
    - python3 ./.travis.make_env_file.py
    - export DOCKER_RUN="docker run --env-file ${ENV_FILE} -v ${TRAVIS_BUILD_DIR}:/root/src/${MY_MODULE} ${IMAGE}"

script:
    - ${DOCKER_RUN} /root/src/${MY_MODULE}/.travis.script.bash

# runs independent of 'script' failure/success
after_script:
    - ${DOCKER_RUN} /root/src/${MY_MODULE}/.travis.after_script.bash

notifications:
  email:
    on_success: change
    on_failure: change
    on_start: never
  webhooks:
    urls:
      - https://buildtimetrend.herokuapp.com/travis
      - https://webhooks.gitter.im/e/2a38e80d2722df87f945

branches:
  except:
    - gh-pages

env:
  global:
    - MY_MODULE=dune-gdt

stages:
    - Test C++
    - Test Python Bindings

jobs:
  include:
#   gcc 6
{%- for c in builders %}
  - stage: test_cpp
    env: DOCKER_TAG=gcc TESTS={{c}}
{%- endfor %}
#   clang 3.9
{%- for c in builders %}
  - stage: test_cpp
    env: DOCKER_TAG=clang TESTS={{c}}
{%- endfor %}
  - stage: test_python
    env: DOCKER_TAG=gcc
    script: ${DOCKER_RUN} /root/src/${MY_MODULE}/.travis.test_python.bash
    # overwrite other global/matrix settings
    after_script: true
  - stage: test_python
    env: DOCKER_TAG=clang
    script: ${DOCKER_RUN} /root/src/${MY_MODULE}/.travis.test_python.bash
    # overwrite other global/matrix settings
    after_script: true

# THIS FILE IS AUTOGENERATED -- DO NOT EDIT #
'''

import os
import jinja2
import sys
import where
import subprocess

tpl = jinja2.Template(tpl)
builder_count = int(sys.argv[1])
ymlfn = os.path.join(os.path.dirname(__file__), '.travis.yml')
with open(ymlfn, 'wt') as yml:
    yml.write(tpl.render(builders=range(0, builder_count)))
travis = where.first('travis')
if travis:
    try:
        subprocess.check_call([str(travis), 'lint', ymlfn])
    except subprocess.CalledProcessError as err:
        print('Linting {} failed'.format(ymlfn))
        print(err)
        sys.exit(-1)
else:
    print('Travis linter missing. Try:\ngem install travis')
