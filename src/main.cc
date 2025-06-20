#include <cstddef>
#include <string>
#include <libheif/heif.h>
#include <emscripten/bind.h>
#include <emscripten/console.h>

struct DecodeOutput {
  int width;
  int height;
  int err;

  // Raw pointers aren't allowed with Embind's value_object, but when pointers go to JS
  // they becomes int (in WASM32) anyways, so...
  int start;

  DecodeOutput() = default;
  DecodeOutput(heif_error_code err): err{err} {}
  DecodeOutput(int width, int height, const uint8_t* data):
    width{width},
    height{height},
    err{heif_error_code::heif_error_Ok},
    start{reinterpret_cast<int>(data)} // ...we just turn it to one in C++!
  {}
};

heif_context* const ctx{heif_context_alloc()};
heif_image* outImg;
uint8_t* inData;

// To maximize efficiency, we are gonna directly use the buffer created in JS pass in and
// return a buffer created in C++, no extra copying. The JS side need to copy that buffer
// in JS somewhere else and then call _freeUsrData to free those memory.
DecodeOutput decode(int start, int size) noexcept {
  heif_error err;
  inData = reinterpret_cast<uint8_t*>(start);
  err = heif_context_read_from_memory_without_copy(ctx, inData, size, nullptr);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;
  
  heif_image_handle* in{};
  err = heif_context_get_primary_image_handle(ctx, &in);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;

  err = heif_decode_image(in, &outImg, heif_colorspace::heif_colorspace_RGB, heif_chroma::heif_chroma_interleaved_RGBA,
    nullptr);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;

  heif_image_handle_release(in);

  int stride;
  return {
    heif_image_get_width(outImg,heif_channel::heif_channel_interleaved),
    heif_image_get_height(outImg,heif_channel::heif_channel_interleaved),
    heif_image_get_plane_readonly(outImg, heif_channel::heif_channel_interleaved, &stride)
  };
}

extern "C" void freeUsrData() noexcept {
  free(inData);
  heif_image_release(outImg);
}

int main() {
  heif_context_set_max_decoding_threads(ctx, 0);
}

using namespace emscripten;
EMSCRIPTEN_BINDINGS(a) {
  value_array<DecodeOutput>("DecodeOutput")
    .element(&DecodeOutput::width)
    .element(&DecodeOutput::height)
    .element(&DecodeOutput::err)
    .element(&DecodeOutput::start);

  function("_decode", &decode);
}