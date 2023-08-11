#!/bin/bash


SOURCE_DIR=$1


if [[ ! -d  ${SOURCE_DIR} ]]; then
  git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git ${SOURCE_DIR}
  cd ${SOURCE_DIR} || exit
  git checkout --force --recurse-submodules "v3.2.2"
fi


