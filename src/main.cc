#include <cstddef>
#include <string>
#include <emscripten/emscripten.h>
#include <libheif/heif.h>
#include <emscripten/console.h>
#include <emscripten/bind.h>

struct DecodeOutput {
  int width;
  int height;
  int err;
  const uint8_t* data;

  DecodeOutput(heif_error_code err): err{err} {}
  DecodeOutput(int width, int height, const uint8_t* data):
    width{width},
    height{height},
    data{data},
    err{heif_error_code::heif_error_Ok}
  {}
};

struct UsrData {
  uint8_t* inData{};
  heif_image* outImg{};
} usrData;

heif_context* const ctx{heif_context_alloc()};

DecodeOutput decode(int start, int size) {
  heif_error err;
  uint8_t* inData{reinterpret_cast<uint8_t*>(start)};
  err = heif_context_read_from_memory_without_copy(ctx, inData, size, nullptr);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;

  usrData.inData = inData;
  
  heif_image_handle* in{};
  err = heif_context_get_primary_image_handle(ctx, &in);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;

  heif_image* out{};
  err = heif_decode_image(in, &out, heif_colorspace::heif_colorspace_RGB, heif_chroma::heif_chroma_interleaved_RGBA,
    nullptr);
  if(err.code != heif_error_code::heif_error_Ok) return err.code;
  usrData.outImg = out;

  heif_image_handle_release(in);

  int stride;
  return {
    heif_image_get_width(out,heif_channel::heif_channel_interleaved),
    heif_image_get_height(out,heif_channel::heif_channel_interleaved),
    heif_image_get_plane_readonly(out, heif_channel::heif_channel_interleaved, &stride)
  };
}

void freeUsrData() {
  free(usrData.inData);
  free(usrData.outImg);
}

using namespace emscripten;
EMSCRIPTEN_BINDINGS() {
  emscripten::enum_<heif_error_code>("ErrorCode")
    .value("Input_does_not_exist", heif_error_Input_does_not_exist)
    .value("Invalid_input", heif_error_Invalid_input)
    .value("Plugin_loading_error", heif_error_Plugin_loading_error)
    .value("Unsupported_filetype", heif_error_Unsupported_filetype)
    .value("Unsupported_feature", heif_error_Unsupported_feature)
    .value("Usage_error", heif_error_Usage_error)
    .value("Memory_allocation_error", heif_error_Memory_allocation_error)
    .value("Decoder_plugin_error", heif_error_Decoder_plugin_error)
    .value("Encoder_plugin_error", heif_error_Encoder_plugin_error)
    .value("Encoding_error", heif_error_Encoding_error)
    .value("Color_profile_does_not_exist", heif_error_Color_profile_does_not_exist)
    .value("Canceled", heif_error_Canceled);
    
  value_array<DecodeOutput>("DecodeOutput")
    .element(&DecodeOutput::width)
    .element(&DecodeOutput::height)
    .element(&DecodeOutput::err);
  function("decode", &decode);
}