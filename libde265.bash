#!/bin/bash

source emsdk/emsdk_env.sh &&

rootDir=$(realpath .)

cd /tmp/libde265 &&
git apply "${rootDir}/libde265.patch" &&
./autogen.sh &&
compileFlags="-Oz -flto -fno-exceptions -DNDEBUG -sSUPPORT_LONGJMP=0 -mmutable-globals -mreference-types -mtail-call -mextended-const -msimd128 -msse4.1"
linkFlags="${compileFlags} -sFILESYSTEM=0 -sWASMFS"
CFLAGS="${compileFlags}" CXXFLAGS="${compileFlags}" LDFLAGS="${linkFlags}"  emconfigure ./configure --enable-static --disable-shared --disable-dec265 --disable-sherlock265 --disable-arm --disable-dependency-tracking --disable-log-error --prefix="${rootDir}/libde265" &&
emmake make -j"$(nproc)" &&
make install