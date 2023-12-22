#!/usr/bin/env bash
pwd=`pwd`
third_party=${pwd}/third-party/RTSP
install_path=${pwd}/third-party-install/RTSP

if [[ ! -d ${third_party} ]]; then
  echo "RTSP not exist, please run git submodule update"
  exit 1
fi

# RTSP
cd ${third_party}               
mkdir RTSP-build
cd RTSP-build
cmake .. -DCMAKE_INSTALL_PREFIX=${install_path} -DCMAKE_TOOLCHAIN_FILE=${pwd}/toolchains/ax620a.cmake
make -j8
make install
cd ${pwd}