#!/bin/bash

source emsdk/emsdk_env.sh &&

LIBDE265=$(realpath libde265)

cd /tmp/libde265 &&
./autogen.sh
compileFlags="-Oz -flto -fno-exceptions -DNDEBUG -sSUPPORT_LONGJMP=0 -mmutable-globals -mreference-types -mtail-call -mextended-const -msimd128"
linkFlags="${compileFlags} -sFILESYSTEM=0 -sWASMFS"
CFLAGS="${compileFlags}" CXXFLAGS="${compileFlags}" LDFLAGS="${linkFlags}"  emconfigure ./configure --enable-static --disable-shared --disable-dec265 --disable-sherlock265 --disable-sse --disable-arm --disable-dependency-tracking --disable-log-error --prefix="${LIBDE265}" &&
emmake make -j"$(nproc)" &&
make install