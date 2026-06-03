# Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y lenguaje
project(PocoRestApiServer VERSION 1.0 LANGUAGES CXX)

# Exigir el estándar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar la biblioteca POCO y los componentes específicos que necesitamos
find_package(Poco REQUIRED COMPONENTS 
    Foundation 
    Net 
    Util 
    Data 
    DataSQLite 
    JSON
)

# Definir el archivo ejecutable a generar
add_executable(api_poco_server main.cpp)

# Enlazar las bibliotecas de POCO al ejecutable
target_link_libraries(api_poco_server 
    PRIVATE 
    Poco::Foundation 
    Poco::Net 
    Poco::Util 
    Poco::Data 
    Poco::DataSQLite 
    Poco::JSON
)
