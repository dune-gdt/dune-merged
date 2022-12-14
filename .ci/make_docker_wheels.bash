#!/usr/bin/env bash

# this script is intended to be run manually

set -exo pipefail

THISDIR="$(cd "$(dirname ${BASH_SOURCE[0]})" && pwd -P )"

cd ${THISDIR}/..
source ./.env

# for rootless/podman execution set both to 0
LOCAL_USER=${LOCAL_USER:$USER}
LOCAL_UID=${LOCAL_UID:-$(id -u)}
LOCAL_GID=${LOCAL_GID:-$(id -g)}
PYTHON_VERSION=${GDT_PYTHON_VERSION:-3.8}

set -eu

IMAGE=${ML_IMAGE_BASE}_py${PYTHON_VERSION}:${ML_TAG}
TEST_IMAGE=docker.io/python:${PYTHON_VERSION}-slim
# check if we a have TTY first, else docker run would throw an error
if [ -t 1 ] ; then
  DT="-t"
else
  DT=""
fi

[[ -e ${THISDIR}/docker ]] || mkdir -p ${THISDIR}/docker
export DOCKER_ENVFILE=${THISDIR}/docker/env
python3 ./deps/scripts/python/make_env_file.py

docker pull -q ${IMAGE}
# this can happen in the background while we build stuff
docker pull -q ${TEST_IMAGE} &

# default command is "build-wheels.sh"
# this deletes testtols and uggrid source dirs
DOCKER_RUN="docker run ${DT} --env-file=${DOCKER_ENVFILE} -e DUNE_SRC_DIR=/home/dxt/src -v ${THISDIR}:/home/dxt/src \
  -e LOCAL_USER=${LOCAL_USER} -e LOCAL_GID=${LOCAL_GID} -e LOCAL_UID=${LOCAL_UID} \
  -i ${IMAGE}"

${DOCKER_RUN} build-wheels.sh

# wait for pull to finish
wait
# makes sure wheels are importable
docker run ${DT} -v ${THISDIR}/docker/wheelhouse/final:/wheelhouse:ro -i ${TEST_IMAGE} \
  bash -c "pip install /wheelhouse/dune* && python -c 'from dune.${md} import *'"

echo '************************************'
echo Wheels are in ${THISDIR}/docker/wheelhouse/final
