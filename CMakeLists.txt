cmake_minimum_required(VERSION 3.18)

project(AGATE_CLI
  VERSION 0.1.0
  LANGUAGES C
)

set(AGATE_UNIT_BASE_DIRECTORY "${CMAKE_INSTALL_PREFIX}/share/agate")
set(AGATE_UNIT_DIRECTORY "${AGATE_UNIT_BASE_DIRECTORY}/units")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_executable(agate
  agate-cli.c
  agate/agate.c
  agate/agate-support.c

  units/src/agate-math-big.c
  units/src/agate-std.c
)

target_compile_features(agate
  PRIVATE
    c_std_99
)

if(MSVC)
  target_compile_options(agate PRIVATE /W3)
else()
  target_compile_options(agate PRIVATE -Wall -Wpedantic)
endif()

target_include_directories(agate
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/agate"
    "${CMAKE_CURRENT_SOURCE_DIR}/units/src"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(agate
  PRIVATE
    m
)

set_target_properties(agate
  PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
)

install(
  TARGETS agate
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/units/units"
  DESTINATION "${AGATE_UNIT_BASE_DIRECTORY}"
)
