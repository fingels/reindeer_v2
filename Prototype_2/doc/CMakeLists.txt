set(DOC_OUTPUT ${PROJECT_BINARY_DIR}/documentation)
file(MAKE_DIRECTORY ${DOC_OUTPUT})

find_program(DOXY_EXECUTABLE doxygen REQUIRED)

configure_file("${PROJECT_SOURCE_DIR}/doc/Doxygen/Doxyfile.in"
               "${PROJECT_BINARY_DIR}/Doxyfile" @ONLY)

add_custom_target(
  documentation
  COMMAND ${DOXY_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${DOC_OUTPUT}
)