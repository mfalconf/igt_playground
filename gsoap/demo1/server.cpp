#include <iostream>
#include "soapService.h" // La clase del servidor autogenerada
#include "ns1.nsmap"     // El mapa de namespaces

// 1. Implementamos la función que definimos en el contrato
int Service::HacerAlgo(char* parametro, int &resultado) {
    std::cout << "\n[Servidor] -> Petición recibida de un cliente." << std::endl;
    std::cout << "[Servidor] -> Mensaje: " << parametro << std::endl;
    
    // Modificamos el resultado. Esto es lo que viajará de vuelta al cliente.
    resultado = 42; 
    
    std::cout << "[Servidor] -> Procesamiento terminado. Enviando respuesta..." << std::endl;
    return SOAP_OK; // Indicamos que no hubo errores
}

int main() {
    // 2. Instanciamos el servidor
    Service servidor;
    int puerto = 8080;

    std::cout << "Iniciando servidor SOAP..." << std::endl;
    std::cout << "Escuchando en el endpoint http://localhost:" << puerto << std::endl;

    // 3. El método run() levanta la red y se queda esperando peticiones infinitamente
    if (servidor.run(puerto) != SOAP_OK) {
        servidor.soap_stream_fault(std::cerr); // Imprime el error si el puerto está ocupado
        return 1;
    }
    
    return 0;
}
