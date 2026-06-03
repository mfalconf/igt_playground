# Buscar los paquetes necesarios de POCO en el sistema
find_package(Poco REQUIRED COMPONENTS Foundation Net Util)

# 1. Configurar el Servidor
add_executable(run_poco_server server.cpp)
target_link_libraries(run_poco_server PRIVATE Poco::Net Poco::Util Poco::Foundation)

# 2. Configurar el Cliente
add_executable(run_poco_client client.cpp)
target_link_libraries(run_poco_client PRIVATE Poco::Net Poco::Foundation)