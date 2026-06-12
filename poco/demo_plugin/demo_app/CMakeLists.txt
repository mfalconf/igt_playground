add_executable(demo_plugin_app main.cpp)
target_include_directories(demo_plugin_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
find_package(Poco REQUIRED Foundation)
target_link_libraries(demo_plugin_app PRIVATE Poco::Foundation)

install(TARGETS demo_plugin_app RUNTIME DESTINATION bin)
