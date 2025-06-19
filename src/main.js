Module = {
  decode(inData) {
    try {
      const inStart = _malloc(inData.length);
      if(inStart === 0) {
        throw "Memory allocation error";
      }
      HEAPU8.set(inData, inStart);
      const [width, height, err, outStart] = Module["_decode"](inStart, inData.length);
      if(err !== 0) {
        throw `libheif error code ${err}`;
      }
      const outData = HEAPU8.subarray(outStart, outStart + width * height * 4);
      return {
        data: new Uint8ClampedArray(outData),
        width: width,
        height: height,
      }
    }
    finally {
      _freeUsrData();
    }
  }
}