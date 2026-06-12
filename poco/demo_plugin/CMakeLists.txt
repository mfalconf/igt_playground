cmake_minimum_required(VERSION 3.14)

project(demo_plugin LANGUAGES CXX)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Poco REQUIRED Foundation)

add_library(demo_plugin SHARED
    ${INCLUDE_DIR}/demo_plugin/Greeter.h
    ${SRC_DIR}/GreeterImpl.cpp
    ${SRC_DIR}/manifest.cpp
)

target_include_directories(demo_plugin PUBLIC ${INCLUDE_DIR})
set_target_properties(demo_plugin PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(demo_plugin PRIVATE Poco::Foundation)

add_subdirectory(demo_app)
add_subdirectory(tests)

install(TARGETS demo_plugin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
