# Búsqueda manual para Spread
find_path(SPREAD_INCLUDE_DIR NAMES sp.h REQUIRED)
find_library(SPREAD_LIBRARY NAMES spread REQUIRED)

add_executable(run_spread main.cpp)

target_include_directories(run_spread PRIVATE ${SPREAD_INCLUDE_DIR})
target_link_libraries(run_spread PRIVATE ${SPREAD_LIBRARY})

add_executable(run_spread_advanced spread_avanzado.cpp)
target_include_directories(run_spread_advanced PRIVATE ${SPREAD_INCLUDE_DIR})
target_link_libraries(run_spread_advanced PRIVATE ${SPREAD_LIBRARY})