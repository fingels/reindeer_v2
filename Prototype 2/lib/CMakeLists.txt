file(GLOB_RECURSE REINDEER_2_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE REINDEER_2_HEADERS CONFIGURE_DEPENDS "*.hpp")

add_library(reindeer2 ${REINDEER_2_SOURCES})

target_compile_features(reindeer2 PRIVATE cxx_std_17)
target_include_directories(reindeer2 PUBLIC ${PROJECT_SOURCE_DIR}/lib/include)
target_compile_features(reindeer2 PRIVATE cxx_std_17)
target_link_libraries(reindeer2 PUBLIC build_type_flags warning_flags)

set_target_properties(reindeer2 PROPERTIES
  FRAMEWORK TRUE
  PUBLIC_HEADER "${REINDEER_2_HEADERS}"
  VERSION "${PROJECT_VERSION}"
  SOVERSION "${PROJECT_VERSION_MAJOR}"
)