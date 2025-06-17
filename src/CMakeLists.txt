add_executable(decoder main.cc)

set(ENABLE_SDL OFF)
set(ENABLE_ENCODER OFF)
set(ENABLE_AOM OFF)
add_subdirectory(../3rdParty/libde265 "${CMAKE_BINARY_DIR}/libde265")
target_link_libraries(decoder PRIVATE de265)

#set(WITH_X265 OFF)
#set(WITH_KVAZAAR OFF)
#set(WITH_OpenH264_DECODER OFF)
#set(WITH_AOM_DECODER OFF)
#set(WITH_AOM_ENCODER OFF)
#set(WITH_LIBSHARPYUV OFF)
#set(BUILD_TESTING OFF)
#set(WITH_GDK_PIXBUF OFF)
#set(LIBDE265_INCLUDE_DIR "${CMAKE_BINARY_DIR}/libde265")
#add_subdirectory(../3rdParty/libheif "${CMAKE_BINARY_DIR}/libheif")
#target_link_libraries(decoder PRIVATE libheif)

add_custom_command(POST_BUILD TARGET decoder
  COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:decoder>"
  "${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:decoder>")