find_package(Git)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "Git not found")
endif()

function(gitClone repo hash outDir)
  file(MAKE_DIRECTORY ${outDir})
  execute_process(WORKING_DIRECTORY ${outDir} COMMAND ${GIT_EXECUTABLE} init)
  execute_process(WORKING_DIRECTORY ${outDir} COMMAND ${GIT_EXECUTABLE} fetch --depth 1
    "https://github.com/${repo}" ${hash})
  execute_process(WORKING_DIRECTORY ${outDir} COMMAND ${GIT_EXECUTABLE} -c advice.detachedHead=false
    checkout ${hash})
  execute_process(WORKING_DIRECTORY ${outDir} COMMAND ${GIT_EXECUTABLE} submodule update --init
   --recursive --depth 1)
endfunction(gitClone repo hash outDir)

# 4.0.12
if(NOT EXISTS emsdk)
  gitClone(emscripten-core/emsdk f39e849effe1bd679aa9ef3cd1798d327c9619db emsdk)
  execute_process(WORKING_DIRECTORY "emsdk" COMMAND ./emsdk install 4.0.12 COMMAND_ERROR_IS_FATAL ANY)
  execute_process(WORKING_DIRECTORY "emsdk" COMMAND ./emsdk activate 4.0.12 COMMAND_ERROR_IS_FATAL ANY)
endif()

# 1.0.16
if(NOT EXISTS libde265)
  file(REMOVE_RECURSE /tmp/libde265/)
  gitClone(strukturag/libde265 7ba65889d3d6d8a0d99b5360b028243ba843be3a /tmp/libde265)
  execute_process(WORKING_DIRECTORY "." COMMAND ./libde265.bash)
endif()

# 1.19.8
if(NOT EXISTS libheif)
  gitClone(strukturag/libheif 5e9deb19fe6b3768af0bb8e9e5e8438b15171bf3 libheif)
  execute_process(WORKING_DIRECTORY "libheif" COMMAND git apply ../libheif.patch)
endif()