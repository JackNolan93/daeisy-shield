cmake_minimum_required(VERSION 3.2)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11.6)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

set (CMAKE_CXX_STANDARD 11)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/pd.build/pd.cmake)

project(resonate-pd-test)

set_pd_sources(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/pure-data/src)
set_pd_external_path(${CMAKE_CURRENT_SOURCE_DIR}/bin/)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resonator~.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ResonatorWrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/DaisySP/Source/PhysicalModeling/resonator.h
)

add_pd_external(resonator resonator~ "${SOURCES}")

SET_SOURCE_FILES_PROPERTIES (${CMAKE_CURRENT_SOURCE_DIR}/resonator~.c PROPERTIES LANGUAGE CXX)

set (WTFSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(resonator PRIVATE "${WTFSOURCES}")

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../lib/DaisySP build)
target_link_libraries (resonator PRIVATE DaisySP)
