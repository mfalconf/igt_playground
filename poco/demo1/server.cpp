#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>

// 1. EL MANEJADOR (Atiende la petición HTTP de un cliente específico)
class MiManejadorHTTP : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override {
        std::cout << "[Servidor] -> Recibida petición " << request.getMethod() 
                  << " desde " << request.clientAddress().toString() << std::endl;

        // Configurar las cabeceras HTTP de respuesta
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        // Enviar el cuerpo de la respuesta (un JSON simple)
        std::ostream& salida = response.send();
        salida << "{\"status\": \"Exito\", \"message\": \"Hola mundo desde el servidor POCO!\"}";
    }
};

// 2. LA FACTORÍA (Decide qué manejador asignar según la ruta, aquí asigna el mismo a todo)
class MiFactoriaManejadores : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override {
        return new MiManejadorHTTP();
    }
};

// 3. LA APLICACIÓN PRINCIPAL (Levanta el servicio y administra el ciclo de vida)
class MiServidorApp : public Poco::Util::ServerApplication {
protected:
    int main(const std::vector<std::string>& args) override {
        unsigned short puerto = 8081;
        
        // Crear el socket de escucha
        Poco::Net::ServerSocket socketEscucha(puerto);
        
        // Configurar y lanzar el servidor HTTP
        Poco::Net::HTTPServer servidor(new MiFactoriaManejadores(), socketEscucha, new Poco::Net::HTTPServerParams);
        servidor.start();
        
        std::cout << "Servidor POCO HTTP iniciado en el puerto " << puerto << "..." << std::endl;
        std::cout << "Presiona Ctrl+C en la terminal para apagarlo." << std::endl;

        // Esperar hasta que se solicite el cierre del programa (Ctrl+C o señal del sistema)
        waitForTerminationRequest();

        std::cout << "Apagando el servidor limpiamente..." << std::endl;
        servidor.stop();

        return Application::EXIT_OK;
    }
};

// Punto de entrada macro de POCO para aplicaciones tipo Demonio/Servicio
POCO_SERVER_MAIN(MiServidorApp)
