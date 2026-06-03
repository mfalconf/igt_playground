#include <iostream>
#include <string>
#include "soapBlackjackBindingProxy.h"
#include "BlackjackBinding.nsmap"      

int main() {
    BlackjackBindingProxy jugador; 
    jugador.soap_endpoint = "http://127.0.0.1:8080"; 

    // 1. Creamos las estructuras (las "cajas") de entrada y salida
    _ns1__PedirCarta solicitud;
    _ns1__PedirCartaResponse respuesta;

    // 2. Llenamos la caja de entrada con los datos del jugador
    solicitud.id_USCOREpartida = 777;

    std::cout << "[Jugador] Pidiendo carta al crupier..." << std::endl;

    // 3. Enviamos ambas cajas por la red. 
    if (jugador.PedirCarta(&solicitud, respuesta) == SOAP_OK) {
        
        // 4. Sacamos el resultado de la caja de respuesta
        std::cout << "[Jugador] Éxito. Me dieron la carta: " << respuesta.nueva_USCOREcarta << std::endl;
        
    } else {
        std::cerr << "[Jugador] Error de comunicación con el casino." << std::endl;
        jugador.soap_stream_fault(std::cerr);
    }

    return 0;
}
