#!/usr/bin/env python3
# kate: indent-width 2;

import os
import jinja2
from itertools import product

tpl = r'''# THIS FILE IS AUTOGENERATED -- DO NOT EDIT       #
# Edit and Re-run .ci/gitlab/config_template.py instead  #
stages:
  - sanity
  - images
{%- for kind in kinds %}
  - {{kind}}
{%- endfor %}
  - python

variables:
    GIT_SUBMODULE_STRATEGY: recursive
    TRAVIS_BRANCH: ${CI_COMMIT_REF_NAME}
    TRAVIS_COMMIT: ${CI_COMMIT_SHA}
    MY_MODULE: dune-gdt
    CCACHE_BASEDIR: ${CI_PROJECT_DIR}
    CCACHE_DIR: "${CI_PROJECT_DIR}/.ccache"
    CCACHE_COMPILERCHECK: content
    CCACHE_COMPRESS: "true"
    PYTEST_ADDOPTS: "-s"
    BASE_PROJECT: zivgitlab.wwu.io/ag-ohlberger/dune-community/dune-gdt-super

.image_builder:
    tags:
      - docker-in-docker
      - long execution time
    stage: images
    rules:
        - if: '$CI_COMMIT_REF_NAME =~ /^staging.*/'
          when: never
        - when: on_success
    retry:
        max: 2
        when:
            - always
    image: harbor.uni-muenster.de/proxy-docker/library/docker:19.03.12
    variables:
        DOCKER_HOST: tcp://docker:2375/
        DOCKER_DRIVER: overlay2
        IMAGE: ${CI_REGISTRY_IMAGE}/ci_testing_${CI_IMAGE}:${CI_COMMIT_SHORT_SHA}
    before_script:
      - |
        docker login -u $CI_REGISTRY_USER -p $CI_REGISTRY_PASSWORD $CI_REGISTRY
        apk --update add py3-pip openssh-client rsync git file bash python3 curl
        pip3 install -U docker jinja2 docopt

        export BASEIMAGE="${MY_MODULE}-testing_${CI_IMAGE}:${CI_COMMIT_REF_NAME/\//_}"
        # get image with fallback to master branch of the super repo
        docker pull ${BASE_PROJECT}/${BASEIMAGE} || export BASEIMAGE="${MY_MODULE}-testing_${CI_IMAGE}:master"
        docker pull ${BASE_PROJECT}/${BASEIMAGE}
    script:
      - |
        git submodule update --init --recursive
        docker build --build-arg BASE=${BASEIMAGE} -t ${IMAGE} -f .ci/gitlab/Dockerfile .
        docker push ${IMAGE}
    services:
        - name: harbor.uni-muenster.de/proxy-docker/library/docker:dind
          alias: docker
    environment:
        name: unsafe

.subdir-test:
    tags:
      - long execution time
    stage: test
    rules:
        - if: '$CI_COMMIT_REF_NAME =~ /^staging.*/'
          when: never
        - when: on_success
    retry:
        max: 2
        when:
            - always
    image: ${CI_REGISTRY_IMAGE}/ci_testing_${CI_IMAGE}:${CI_COMMIT_SHORT_SHA}
    cache:
      key: "$CI_JOB_NAME"
      paths:
        - .ccache
    before_script:
      - |
        mkdir /home/dune-ci/testresults && chmod -R 777 /home/dune-ci/testresults
        [[ -f ./.gitsuper ]] && echo "Please remove .gitsuper from the repo" && exit 1
        ccache --zero-stats || true
    after_script:
      - ccache --show-stats
    artifacts:
      reports:
        junit: '/home/dune-ci/testresults/*xml'
    environment:
        name: unsafe

.pre-commit:
    stage: sanity
    image: "python:3.9"
    cache:
      paths:
        - ${CI_PROJECT_DIR}/.cache/pre-commit
    variables:
        PRECOMMIT: "pre-commit run --all"
    before_script:
        - pip install pre-commit
        - pre-commit install --install-hooks

format_check:
    extends: .pre-commit
    script:
        - ${PRECOMMIT} clang-format
        - ${PRECOMMIT} yapf
        - ${PRECOMMIT} trailing-whitespace

lint:
    extends: .pre-commit
    script:
        - ${PRECOMMIT} check-added-large-files
        #- ${PRECOMMIT} flake8 disabled for now, finds lots of issue
        - ${PRECOMMIT} debug-statements
        - ${PRECOMMIT} check-yaml


{% for image in images -%}
{{ image }}:
    extends: .image_builder
    variables:
        CI_IMAGE: {{ image }}
{% endfor %}

{% for image, subdir, kind in matrix %}
{{ subdir }} {{ image[image.find('debian')+1+6:] }} {{kind}}:
    extends: .subdir-test
    variables:
        CI_IMAGE: {{ image }}
        TESTS_MODULE_SUBDIR: {{ subdir }}
    tags:
        - dustin
    stage: {{kind}}
    needs: ["{{image}}"]
    script:
          - /home/dune-ci/src/${MY_MODULE}/.ci/shared/scripts/test_{{kind}}.bash
{% endfor %}

{% for image in images %}
{{ image[image.find('debian')+1+6:] }} python:
    extends: .subdir-test
    variables:
        CI_IMAGE: {{ image }}
    tags:
    {%- if 'clang' in image %}
        - amm-only
    {%- else %}
        - dustin
    {%- endif %}
    stage: python
    needs: ["{{image}}"]
    script:
          - /home/dune-ci/src/${MY_MODULE}/.ci/shared/scripts/test_python.bash
{% endfor %}

'''


tpl = jinja2.Template(tpl)
images = ['debian-unstable_gcc_full', 'debian_gcc_full', 'debian_clang_full']
subdirs = ['gdt']
kinds = ['cpp', 'headercheck']
matrix = product(images, subdirs, kinds)
with open(os.path.join(os.path.dirname(__file__), 'config.yml'), 'wt') as yml:
    yml.write(tpl.render(matrix=matrix, images=images, kinds=kinds))
