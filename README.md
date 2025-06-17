# Heic-D-Code
A wasm build of libheif whose only purpose is to decode `.heif` image or the first image of a `.heic` to make it super small so you can render those on browsers.
And yes, the name is inspired by One Piece.


# Building instruction (from scratch)
```bash
git clone --depth 1 https://github.com/msqr1/SnapFridge-heif &&
cd SnapFridge-heif &&

git clone --depth 1 https://github.com/emscripten-core/emsdk &&
cd emsdk &&
./emsdk install 4.0.10 &&
./emsdk activate 4.0.10 &&
cd .. &&

cd 3rdParty &&
cmake -P Setup.cmake &&
cd .. &&

mkdir build &&
cd build &&

source ../emsdk/emsdk_env.sh &&
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release &&
cmake --build . --config Release -j $(cmake -P ../nproc.cmake)
```