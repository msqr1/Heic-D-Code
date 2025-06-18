#!/bin/bash

source emsdk/emsdk_env.sh &&

LIBDE265=$(realpath libde265)

cd /tmp/libde265 &&
./autogen.sh
CXXFLAGS="-O3 -flto -fno-exceptions -msimd128" CFLAGS="-O3 -flto -fno-exceptions -msimd128" LDFLAGS="-O3 -flto" emconfigure ./configure --enable-static --disable-shared --disable-dec265 --disable-sherlock265 --disable-sse --prefix="${LIBDE265}" &&
emmake make -j"$(nproc)" &&
make install
