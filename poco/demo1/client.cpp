#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>
#include <iostream>

int main() {
    std::string ip = "127.0.0.1";
    unsigned short puerto = 8081;

    try {
        std::cout << "[Cliente] -> Conectando a " << ip << ":" << puerto << "..." << std::endl;
        
        // 1. Crear la sesión HTTP con el servidor
        Poco::Net::HTTPClientSession sesion(ip, puerto);

        // 2. Preparar una petición GET HTTP/1.1 para la ruta raíz "/"
        Poco::Net::HTTPRequest peticion(Poco::Net::HTTPRequest::HTTP_GET, "/", Poco::Net::HTTPMessage::HTTP_1_1);
        
        // 3. Enviar la petición al servidor
        sesion.sendRequest(peticion);

        // 4. Recibir la respuesta del servidor
        Poco::Net::HTTPResponse respuesta;
        std::istream& flujoEntrada = sesion.receiveResponse(respuesta);

        // 5. Mostrar datos de la respuesta en consola
        std::cout << "[Cliente] -> Código de estado: " << respuesta.getStatus() 
                  << " " << respuesta.getReason() << std::endl;
        std::cout << "[Cliente] -> Tipo de contenido: " << respuesta.getContentType() << std::endl;
        
        std::cout << "[Cliente] -> JSON Recibido: ";
        // Copiar el flujo de red directamente al flujo de consola estándar
        Poco::StreamCopier::copyStream(flujoEntrada, std::cout);
        std::cout << std::endl;

    } catch (Poco::Exception& ex) {
        std::cerr << "[Cliente] -> Error de red en POCO: " << ex.displayText() << std::endl;
        return 1;
    }

    return 0;
}
