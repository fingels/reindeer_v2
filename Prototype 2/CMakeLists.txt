cmake_minimum_required(VERSION 3.21)
project(reindeer2 VERSION 0.0.1 LANGUAGES CXX)

option(REINDEER_PORTABLE "Portable x86-64 build" OFF)
option(REINDEER_ERROR "Turn warning into errors." OFF)

add_library(build_type_flags INTERFACE)
target_compile_options(build_type_flags INTERFACE
  $<$<CONFIG:Debug>:-O -g>
  $<$<CONFIG:Release>:-O3 -DNDEBUG>
  $<$<CONFIG:Profile>:-O3 -ggdb3 -DNDEBUG -fno-inline>
  $<$<CONFIG:Coverage>:-O0 -g --coverage -fprofile-abs-path>
  $<IF:$<BOOL:${REINDEER_PORTABLE}>,-march=x86-64,-march=native>
  $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
  $<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>
)

add_library(warning_flags INTERFACE)
target_compile_options(warning_flags INTERFACE
  "-Wall"
  "-Wextra"
  "-pedantic"
  $<$<CONFIG:Release>:$<$<BOOL:${REINDEER_ERROR}>:-Werror>>
)

add_subdirectory(app)