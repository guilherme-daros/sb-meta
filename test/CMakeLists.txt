cmake_minimum_required(VERSION 3.10)
project(protocol-unit-tests)

add_executable(protocol-unit-tests Main.cpp)


target_compile_features(protocol-unit-tests PRIVATE cxx_std_17)

target_link_libraries(protocol-unit-tests PUBLIC 
  gtest 
  gtest_main 
  )

target_link_libraries(protocol-unit-tests INTERFACE 
  infra
  protocol
  )


target_include_directories(protocol-unit-tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(UNIT_TEST protocol-unit-tests)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})
add_custom_target(
  run_unit_test ALL
  COMMAND ${UNIT_TEST} --output-on-failure --gtest_output="xml:gtest.xml"
  DEPENDS ${UNIT_TEST}
  )
