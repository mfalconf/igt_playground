#include <iostream>
#include <sqlite3.h>
#include <string>

int main() {
    sqlite3* base_datos = nullptr;
    char* mensaje_error = nullptr;

    // 1. ABRIR (O CREAR) LA BASE DE DATOS
    // Si "mi_app.db" no existe, SQLite lo crea como un archivo nuevo en la carpeta.
    int estado = sqlite3_open("mi_app.db", &base_datos);
    
    if (estado != SQLITE_OK) {
        std::cerr << "Error al abrir DB: " << sqlite3_errmsg(base_datos) << std::endl;
        return 1;
    }
    std::cout << "[INFO] Base de datos abierta exitosamente." << std::endl;

    // 2. CREAR UNA TABLA
    const char* sql_crear_tabla = 
        "CREATE TABLE IF NOT EXISTS Usuarios ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Nombre TEXT NOT NULL, "
        "Edad INTEGER);";

    estado = sqlite3_exec(base_datos, sql_crear_tabla, nullptr, nullptr, &mensaje_error);
    if (estado != SQLITE_OK) {
        std::cerr << "Error creando tabla: " << mensaje_error << std::endl;
        sqlite3_free(mensaje_error);
    }

    // 3. INSERTAR DATOS
    const char* sql_insertar = "INSERT INTO Usuarios (Nombre, Edad) VALUES ('Carlos', 35);";
    estado = sqlite3_exec(base_datos, sql_insertar, nullptr, nullptr, &mensaje_error);
    if (estado != SQLITE_OK) {
        std::cerr << "Error insertando: " << mensaje_error << std::endl;
        sqlite3_free(mensaje_error);
    }

    // 4. LEER DATOS (La forma profesional con sentencias preparadas)
    const char* sql_leer = "SELECT ID, Nombre, Edad FROM Usuarios;";
    sqlite3_stmt* sentencia; // El objeto que guarda la consulta compilada

    // Preparamos la consulta
    estado = sqlite3_prepare_v2(base_datos, sql_leer, -1, &sentencia, nullptr);
    
    if (estado == SQLITE_OK) {
        std::cout << "\n--- Registros en la Base de Datos ---" << std::endl;
        
        // Ejecutamos paso a paso (fila por fila)
        while (sqlite3_step(sentencia) == SQLITE_ROW) {
            int id = sqlite3_column_int(sentencia, 0); // Columna 0: ID
            
            // Los textos en SQLite son unsigned char*, hay que hacer un cast a char* para C++
            const char* nombre = reinterpret_cast<const char*>(sqlite3_column_text(sentencia, 1)); 
            
            int edad = sqlite3_column_int(sentencia, 2); // Columna 2: Edad

            std::cout << "ID: " << id << " | Nombre: " << nombre << " | Edad: " << edad << std::endl;
        }
    } else {
        std::cerr << "Error al leer datos." << std::endl;
    }

    // 5. LIMPIAR MEMORIA Y CERRAR
    sqlite3_finalize(sentencia); // Destruye la consulta compilada
    sqlite3_close(base_datos);   // Cierra el archivo
    std::cout << "[INFO] Base de datos cerrada." << std::endl;

    return 0;
}
