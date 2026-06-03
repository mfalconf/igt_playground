#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    int rc = sqlite3_open("prueba.db", &db);
    
    if (rc) {
        std::cerr << "Error al abrir DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Base de datos abierta/creada exitosamente" << std::endl;
    }
    
    // Crear tabla
    const char* sql = "CREATE TABLE IF NOT EXISTS Usuarios (ID INT, Nombre TEXT);";
    char* errorMsg = nullptr;
    rc = sqlite3_exec(db, sql, 0, 0, &errorMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error SQL: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
    
    sqlite3_close(db);
    return 0;
}
