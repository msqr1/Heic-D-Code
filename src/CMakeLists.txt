add_executable(decoder main.cc)
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

add_compile_definitions(__EMSCRIPTEN_STANDALONE_WASM__)
set(WITH_X265 OFF)
set(WITH_KVAZAAR OFF)
set(WITH_OpenH264_DECODER OFF)
set(WITH_AOM_DECODER OFF)
set(WITH_AOM_ENCODER OFF)
set(WITH_LIBSHARPYUV OFF)
set(BUILD_TESTING OFF)
set(WITH_GDK_PIXBUF OFF)
set(WITH_EXAMPLES OFF)
set(LIBDE265_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libde265/include")
set(LIBDE265_LIBRARY "${CMAKE_SOURCE_DIR}/libde265/lib/libde265.a")
add_subdirectory("${CMAKE_SOURCE_DIR}/libheif" "${CMAKE_BINARY_DIR}/libheif")
target_link_libraries(decoder PRIVATE heif)

add_custom_command(POST_BUILD TARGET decoder
  COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:decoder>"
  "${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:decoder>")