#include <iostream>
#include "soapBlackjackBindingService.h"
#include "BlackjackBinding.nsmap"        

// Implementamos directamente la función de la clase generada por gSOAP.
int BlackjackBindingService::PedirCarta(_ns1__PedirCarta *solicitud, _ns1__PedirCartaResponse &respuesta) {
    
    // 1. Sacamos el dato de la estructura de entrada
    int partida = solicitud->id_USCOREpartida; 
    
    std::cout << "[Casino] El jugador de la partida " << partida << " pidió una carta." << std::endl;
    
    // 2. Metemos el resultado en la estructura de salida
    respuesta.nueva_USCOREcarta = "As de Picas"; 
    
    std::cout << "[Casino] Enviando carta: " << respuesta.nueva_USCOREcarta << std::endl;
    
    return SOAP_OK; 
}

int main() {
    // Instanciamos directamente el servicio generado por gSOAP
    BlackjackBindingService servidor_casino;
    int puerto = 8080;

    std::cout << "Iniciando Servidor de Blackjack en el puerto " << puerto << "..." << std::endl;
    
    // Iniciamos la red
    if (servidor_casino.run(puerto) != SOAP_OK) {
        servidor_casino.soap_stream_fault(std::cerr);
    }
    
    return 0;
}
