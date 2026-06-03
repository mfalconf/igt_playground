cmake_minimum_required(VERSION 3.12)
project(BlackjackGame CXX)

# Forzar el estándar de C++ (gSOAP moderno funciona excelente con C++11 en adelante)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. BUSCAR GSOAP EN EL SISTEMA (WSL / Ubuntu)
# Buscamos el archivo de cabecera principal de gSOAP
find_path(GSOAP_INCLUDE_DIR NAMES stdsoap2.h
    PATHS /usr/include /usr/local/include
)

# Buscamos el archivo binario de la biblioteca C++ (libgsoap++.a o .so)
find_library(GSOAP_LIBRARY NAMES gsoap++
    PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
)

# Verificación de seguridad: si no encuentra la lib, detiene el proceso con un error claro
if(NOT GSOAP_INCLUDE_DIR OR NOT GSOAP_LIBRARY)
    message(FATAL_ERROR "¡Error! No se encontró gSOAP en el sistema. 
    Asegúrate de haber ejecutado: sudo apt install libgsoap-dev")
endif()

message(STATUS "gSOAP Encontrado:")
message(STATUS "  Headers: ${GSOAP_INCLUDE_DIR}")
message(STATUS "  Library: ${GSOAP_LIBRARY}")

# 2. CONFIGURAR EL SERVIDOR (Casino)
add_executable(casino_server 
    casino_server.cpp 
    soapC.cpp 
    soapBlackjackBindingService.cpp
)

# Decirle al servidor dónde buscar tanto tus archivos .h como los de gSOAP
target_include_directories(casino_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GSOAP_INCLUDE_DIR}
)

# Enlazar físicamente la biblioteca encontrada al servidor
target_link_libraries(casino_server PRIVATE ${GSOAP_LIBRARY})


# 3. CONFIGURAR EL CLIENTE (Jugador)
add_executable(jugador_client 
    jugador_client.cpp 
    soapC.cpp 
    soapBlackjackBindingProxy.cpp
)

# Decirle al cliente dónde buscar tanto tus archivos .h como los de gSOAP
target_include_directories(jugador_client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GSOAP_INCLUDE_DIR}
)

# Enlazar físicamente la biblioteca encontrada al cliente
target_link_libraries(jugador_client PRIVATE ${GSOAP_LIBRARY})
