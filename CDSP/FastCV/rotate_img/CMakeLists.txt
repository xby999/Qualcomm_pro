# Sets the minimum version of CMake required to build your native library.
# This ensures that a certain set of CMake features is available to
# your build.

cmake_minimum_required(VERSION 3.14.3)
project(benchmark C CXX ASM)
enable_language(ASM)
cmake_policy(SET CMP0115 OLD)

if(HEXAGON_SDK_ROOT)
    include(${HEXAGON_SDK_ROOT}/build/cmake/hexagon_fun.cmake)
else()
    include(${HEXAGON_CMAKE_ROOT}/hexagon_fun.cmake)
endif()
set(ignoreVar ${DEPENDENCIES}${V}${BUILD_NAME}${BUILD_TAG_NAME})

set(common_incs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_BINARY_DIR}/
    ${HEXAGON_SDK_ROOT}/incs/
    ${HEXAGON_SDK_ROOT}/incs/stddef/
    ${HEXAGON_SDK_ROOT}/utils/examples/
    )

include_directories(${common_incs})

if(${OS_TYPE} MATCHES "HLOS")

    add_library(corner_detect
                SHARED ${CMAKE_CURRENT_BINARY_DIR}/corner_detect_stub
                ${HEXAGON_SDK_ROOT}/utils/examples/dsp_capabilities_utils
                )

    buildIDL(inc/corner_detect.idl corner_detect)

    choose_dsprpc(${DSP_TYPE} dsprpc)

    add_executable(corner_detect_device ${CMAKE_CURRENT_SOURCE_DIR}/src/corner_detect)

    set_target_properties(corner_detect_device PROPERTIES OUTPUT_NAME "corner_detect")

    link_options(corner_detect_device)

    target_compile_definitions(corner_detect_device PUBLIC VERIFY_PRINT_ERROR)

    link_custom_library(corner_detect_device rpcmem)
    link_custom_library(corner_detect ${dsprpc})
    link_custom_library(corner_detect_device ${dsprpc})

    add_dependencies(corner_detect_device corner_detect)
    target_link_libraries(corner_detect_device corner_detect)

    # Copy binaries from ship to CMAKE_CURRENT_BINARY_DIR folder
    copy_binaries(corner_detect_device corner_detect)

else()

    set(srcs_dsp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/corner_detect_imp
        ${CMAKE_CURRENT_BINARY_DIR}/corner_detect_skel
        )

    add_library(corner_detect_skel
                SHARED ${srcs_dsp}
                ${HEXAGON_SDK_ROOT}/utils/examples/dsp_capabilities_utils
                )

    buildIDL(inc/corner_detect.idl corner_detect_skel)

    target_compile_definitions(corner_detect_skel PUBLIC FASTCV_NO_INLINE_OPTIONS _MACH_I32=int)

    # Copy binaries from ship to CMAKE_CURRENT_BINARY_DIR folder
    copy_binaries(corner_detect_skel)

endif()
