cmake_minimum_required(VERSION 3.10)
set(TEST_PROJECT_NAME sb-meta-tests)

project(${TEST_PROJECT_NAME})

add_executable(${TEST_PROJECT_NAME} Main.cpp)

target_link_libraries(
  ${TEST_PROJECT_NAME} PUBLIC 
  gtest 
  gtest_main 
  )

target_link_libraries(
  ${TEST_PROJECT_NAME} PUBLIC 
  sb::meta
  )

add_subdirectory(TypeFinder)

set(UNIT_TEST ${TEST_PROJECT_NAME})
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

add_custom_target(
  run_unit_test ALL
  COMMAND ${UNIT_TEST} --gtest_color=yes --output-on-failure --gtest_output="xml:gtest.xml"
  DEPENDS ${UNIT_TEST}
  )
