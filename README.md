# Heic-D-Code
- A wasm build of libheif whose only purpose is to decode `.heif` image or the first image of a `.heic` to make it super small so you can render those on browsers. Its size is half of other libraries like `heic2any` or `heic-to`.
- Input: An `Uint8Array` containing HEIC image data.
- Output: An `ImageData`-like object, with the exact same fields, minus `settings`, so that it's transferrable because `ImageData` isn't transferrable.
- Used in [SnapFridge](https://github.com/msqr1/SnapFridge)
- Version: libde265 1.0.16, libheif 1.19.8

# Building instruction (from scratch)
```bash
git clone --depth 1 https://github.com/msqr1/Heic-D-Code &&
cd Heic-D-Code &&
cmake -P Setup.cmake &&
source emsdk/emsdk_env.sh &&
mkdir build &&
cd build &&
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release &&
cmake --build . --config Release -j$(nproc)
```

# Usage
```mjs
import initDecoder from "./Heic-D-Code.js";

const decoder = await initDecoder();
const canvas = document.createElement("canvas");
const context = canvas.getContext("2d");

try {
  const heic = await fetch("example.heic");
  const inData = await heic.bytes();
  const { data, width, height } = decoder.decode(inData);
  canvas.width = width;
  canvas.height = height;
  ctx.putImageData(new ImageData(data, width, height), 0, 0);
}
catch(e) {
  console.log(e);
}
```