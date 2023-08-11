#!/bin/bash


SOURCE_DIR=$1


if [[ ! -d  ${SOURCE_DIR} ]]; then
  git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git ${SOURCE_DIR}/source
  mkdir -p ${SOURCE_DIR}/build
  mkdir -p ${SOURCE_DIR}/install/lib


  cd ${SOURCE_DIR}/source || exit
  git checkout --force --recurse-submodules "v3.2.2"
fi


