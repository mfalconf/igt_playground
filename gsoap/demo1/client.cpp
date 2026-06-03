#include <iostream>
#include "soapProxy.h" // La clase del cliente autogenerada
#include "ns1.nsmap"   // El mapa de namespaces

int main() {
    // La clase generada se llama Proxy
    Proxy servicio;

    char parametro[] = "Mensaje de prueba para gsoap";
    int resultado = 0;

    // LE DECIMOS AL CLIENTE A QUÉ PUERTO CONECTARSE
    servicio.soap_endpoint = "http://localhost:8080";

    std::cout << "Llamando al servicio SOAP..." << std::endl;

    if (servicio.HacerAlgo(parametro, resultado) == SOAP_OK) {
        std::cout << "Llamada exitosa. Resultado: " << resultado << std::endl;
    } else {
        std::cerr << "Error en la llamada SOAP." << std::endl;
        servicio.soap_stream_fault(std::cerr);
    }

    return 0;
}
