# Búsqueda de cabeceras
find_path(GSOAP_INCLUDE_DIR 
    NAMES stdsoap2.h 
    PATHS /usr/include /usr/local/include /usr/share/gsoap
    PATH_SUFFIXES gsoap
    REQUIRED
)

# Búsqueda ESTRICTA de la biblioteca en C++
find_library(GSOAP_LIBRARY 
    NAMES gsoap++ 
    PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib
    REQUIRED
)

# Añadimos los archivos del cliente
add_executable(run_gsoap_client 
    client.cpp 
    soapC.cpp 
    soapProxy.cpp 
)

# Directorios de inclusión
target_include_directories(run_gsoap_client PRIVATE 
    ${GSOAP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(run_gsoap_client PRIVATE ${GSOAP_LIBRARY})

# Añadimos los archivos del servidor
add_executable(run_gsoap_server 
    server.cpp 
    soapC.cpp 
    soapService.cpp 
)

target_include_directories(run_gsoap_server PRIVATE 
    ${GSOAP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(run_gsoap_server PRIVATE ${GSOAP_LIBRARY})

# (Nota: Para gSOAP, recuerda que tu main.cpp probablemente 
# dependerá de los archivos .cpp y .h generados por soapcpp2.
# Si es así, debes agregarlos al ejecutable: 
# add_executable(run_gsoap main.cpp soapC.cpp soapClient.cpp)).