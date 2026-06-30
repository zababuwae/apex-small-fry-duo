cmake_minimum_required(VERSION 3.25)

project(ApexSmallFryDuo VERSION 0.1.0 LANGUAGES CXX)

#-collect sources with glob
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
add_executable(ApexSmallFryDuo WIN32 ${sources})

#-match visual studio explorer with actual folder structure
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES ${sources})

#-set include root instead of using relative path
target_include_directories(ApexSmallFryDuo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")


target_compile_features(ApexSmallFryDuo PRIVATE cxx_std_17)
set_target_properties(ApexSmallFryDuo PROPERTIES
    CXX_EXTENSIONS OFF
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

target_compile_definitions(ApexSmallFryDuo PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

if(MSVC)
    target_compile_options(ApexSmallFryDuo PRIVATE
        /utf-8
        /W4
        /permissive-
        /Zc:__cplusplus
    )

    set_property(TARGET ApexSmallFryDuo PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    )
endif()

target_link_libraries(ApexSmallFryDuo PRIVATE
    d3d11
    dxgi
)

set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY
    VS_STARTUP_PROJECT ApexSmallFryDuo
)
