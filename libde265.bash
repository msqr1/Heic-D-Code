#!/bin/bash

source emsdk/emsdk_env.sh &&

LIBDE265=$(realpath libde265)

cd /tmp/libde265 &&
#./autogen.sh
FLAGS="-Oz -flto -fno-exceptions -DNDEBUG -sSUPPORT_LONGJMP=0"
CFLAGS="${FLAGS}" CXXFLAGS="${FLAGS}" LDFLAGS="${FLAGS} -sEVAL_CTORS=2"  emconfigure ./configure --enable-static --disable-shared --disable-dec265 --disable-sherlock265 --disable-sse --disable-arm --disable-dependency-tracking --disable-log-error --prefix="${LIBDE265}" &&
emmake make -j"$(nproc)" &&
make install