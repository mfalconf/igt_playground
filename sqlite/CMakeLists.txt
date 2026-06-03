find_package(SQLite3 REQUIRED)

add_executable(run_sqlite main.cpp)
target_link_libraries(run_sqlite PRIVATE SQLite::SQLite3)

add_executable(run_sqlite_advanced sqlite_avanzado.cpp)
target_link_libraries(run_sqlite_advanced PRIVATE sqlite3)