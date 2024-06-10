option(USE_VERBOSE_SHADER_OUTPUT "" OFF)
if(USE_VERBOSE_SHADER_OUTPUT)
  set(VERBOSE_SHADER_OUTPUT --verbose)
else()
  set(QUIET_SHADER_OUTPUT ERROR_QUIET)
endif()

# cmake-format: off
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/shader/build)
execute_process(
  COMMAND
    third-party/install/bin/shaderc
    -f shader/vs_vertcol.sc
    -o shader/build/vs_vertcol.bin
    --platform osx
    --type vertex
    -i ./
    -p metal
    ${VERBOSE_SHADER_OUTPUT}
  ${QUIET_SHADER_OUTPUT}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(
  COMMAND
    third-party/install/bin/shaderc
    -f shader/fs_vertcol.sc
    -o shader/build/fs_vertcol.bin
    --platform osx
    --type fragment
    -i ./
    -p metal
    ${VERBOSE_SHADER_OUTPUT}
  ${QUIET_SHADER_OUTPUT}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
# cmake-format: on
