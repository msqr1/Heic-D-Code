cmake_minimum_required(VERSION 3.25)
project(Heif-D-Code CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(FATAL_ERROR "In-source build is unsupported")
endif()
  
set(commonCompileFlags "-fno-exceptions;
  -Wno-everything;
  -mmutable-globals;
  -mreference-types;
  -mtail-call;
  -mextended-const;
  -msimd128;
  -sSUPPORT_LONGJMP=0;
  -sSTRICT;")
set(commonLinkFlags "-sMALLOC=emmalloc;
  -sENVIRONMENT=web,worker;
  -sEXPORTED_FUNCTIONS=_malloc,_freeUsrData,_main;
  -sEXPORT_ES6;
  -sMODULARIZE;
  -sALLOW_MEMORY_GROWTH;
  -sPOLYFILL=0;
  -sEXIT_RUNTIME=0;
  -lembind;
  -sERROR_ON_UNDEFINED_SYMBOLS=0;
  -sDYNAMIC_EXECUTION=0;
  -sEMBIND_AOT;
  -sEXPORT_KEEPALIVE=0;
  --pre-js;${CMAKE_SOURCE_DIR}/src/main.js;")
set(debugCompileFlags "${commonCompileFlags};
  -fsanitize=address;
  -fsanitize=undefined;
  -O0")
set(debugLinkFlags "${debugCompileFlags};${commonLinkFlags};
  -sASSERTIONS=2;
  -sSTACK_OVERFLOW_CHECK=2;
  -sINITIAL_MEMORY=405995520;
  -sWASMFS")
set(releaseCompileFlags "${commonCompileFlags};
  -Oz;
  -flto;")
set(releaseLinkFlags "${releaseCompileFlags};${commonLinkFlags};
  -sEVAL_CTORS=2;
  -sFILESYSTEM=0;
  -sINITIAL_MEMORY=2293760;
  --closure;1")

# The Release mode initial memory is just over the memory for the app itself plus 2mb for a photo.

get_cmake_property(multiConfigGenerator GENERATOR_IS_MULTI_CONFIG)
if(multiConfigGenerator)
  message("Multi-config generator detected")
  add_compile_options("$<$<CONFIG:Debug>:${debugCompileFlags}>"
    "$<$<CONFIG:Release>:${releaseCompileFlags}>")
  add_link_options("$<$<CONFIG:Debug>:${debugLinkFlags}>"
    "$<$<CONFIG:Release>:${releaseLinkFlags}>")
else()
  message("Single-config generator detected")
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(${debugCompileFlags})
    add_link_options(${debugLinkFlags})
  elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(${releaseCompileFlags})
    add_link_options(${releaseLinkFlags})
  else()
    message(FATAL_ERROR "Unknown build type, only Debug/Release are allowed")
  endif()
endif()
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(src)
