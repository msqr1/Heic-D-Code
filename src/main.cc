#include <cstddef>
#include <emscripten/emscripten.h>
#include <libheif/heif.h>

struct DecodeOutput {
  const uint8_t* data;
  size_t size;
  unsigned int width;
  unsigned int height;
};

struct UsrData {
  std::byte* inData{};
  heif_image* outImg{};
} usrData;

heif_context* const ctx = heif_context_alloc();

extern "C" {
  EMSCRIPTEN_KEEPALIVE DecodeOutput decode(size_t start, size_t size) {
    std::byte* inData{reinterpret_cast<std::byte*>(start)};
    heif_context_read_from_memory_without_copy(ctx, inData, size, nullptr);
    usrData.inData = inData;

    heif_image_handle* in;
    heif_context_get_primary_image_handle(ctx, &in);

    heif_image* out;
    heif_decode_image(in, &out, heif_colorspace::heif_colorspace_RGB, heif_chroma::heif_chroma_interleaved_RGBA,
      nullptr);
    usrData.outImg = out;

    int stride;
    const uint8_t* outData = heif_image_get_plane_readonly(out, heif_channel::heif_channel_interleaved, &stride);
    const unsigned int width = heif_image_get_width(out,heif_channel::heif_channel_interleaved);
    const unsigned int height = heif_image_get_height(out,heif_channel::heif_channel_interleaved);
    heif_image_handle_release(in);
    return {
      outData,
      width * height * 3,
      width,
      height
    };
  }

  EMSCRIPTEN_KEEPALIVE void freeUsrData() {
    free(usrData.inData);
    free(usrData.outImg);
  }
}
