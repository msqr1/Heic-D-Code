#include <cstddef>
#include <emscripten/emscripten.h>
#include <libheif/heif.h>

struct ImageData {
  std::byte* data;
  size_t len;
  unsigned int width;
  unsigned int height;
};

extern "C" EMSCRIPTEN_KEEPALIVE ImageData decode() {
  
}