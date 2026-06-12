cmake_minimum_required(VERSION 3.14)

find_package(GTest QUIET)
find_package(Poco REQUIRED Foundation)

if (GTest_FOUND)
  add_executable(demo_plugin_tests test_greeter.cpp)
  target_include_directories(demo_plugin_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(demo_plugin_tests PRIVATE GTest::gtest_main Poco::Foundation)
  # Ensure the plugin target is built before the tests and pass its absolute path to the test
  add_dependencies(demo_plugin_tests demo_plugin)
  add_test(NAME PluginTest COMMAND demo_plugin_tests $<TARGET_FILE:demo_plugin>)
else()
  message(WARNING "GTest not found. Building a stub test target that prints a message. To run real tests, install googletest and re-run CMake.")
  add_executable(demo_plugin_tests_stub test_stub.cpp)
  target_include_directories(demo_plugin_tests_stub PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  add_test(NAME PluginTestStub COMMAND demo_plugin_tests_stub)
endif()
