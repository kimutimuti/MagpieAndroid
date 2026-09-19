cmake_minimum_required(VERSION 3.22.1)

project("magpie")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    magpie
    SHARED
    native-lib.cpp
    Renderer.cpp
    ShaderUtils.cpp
    effects/Anime4K.cpp
    effects/FSR.cpp
    effects/Bicubic.cpp
    effects/Lanczos.cpp
)

find_library(
    log-lib
    log
)

find_library(
    EGL-lib
    EGL
)

find_library(
    GLESv3-lib
    GLESv3
)

target_link_libraries(
    magpie
    ${log-lib}
    ${EGL-lib}
    ${GLESv3-lib}
    android
)
