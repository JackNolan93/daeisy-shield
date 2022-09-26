cmake_minimum_required(VERSION 3.2)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11.6)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/pd.build/pd.cmake)

project(wtf-osc-pd-test)

set_pd_sources(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/pure-data/src)
set_pd_external_path(${CMAKE_CURRENT_SOURCE_DIR}/bin/)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wtfosc~.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wtfosc_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dsp/WTFOscillator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dsp/WTFOscillator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/dsp/WTFWindow.h
)

add_pd_external(wtfosc wtfosc~ "${SOURCES}")

SET_SOURCE_FILES_PROPERTIES( ${CMAKE_CURRENT_SOURCE_DIR}/wtfosc~.c PROPERTIES LANGUAGE CXX )

set(WTFSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../src/dsp)

target_include_directories(wtfosc PRIVATE "${WTFSOURCES}")
