add_executable(reindeer2-main main.cpp)
target_compile_features(reindeer2-main PRIVATE cxx_std_17)
target_link_libraries(reindeer2-main PRIVATE build_type_flags)
target_link_libraries(reindeer2-main PRIVATE reindeer2)

set_target_properties(reindeer2-main PROPERTIES
  OUTPUT_NAME "reindeer2"
)