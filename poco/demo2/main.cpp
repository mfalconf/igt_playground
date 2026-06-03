#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/URI.h>
#include <iostream>
#include <string>
#include <vector>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::Data::Keywords;
using namespace Poco;

// Estructura de Dominio que mapea la entidad de la Base de Datos
struct Usuario {
    int id = 0;
    std::string name;
    std::string username;
    std::string email;
};

// ============================================================================
// CONTROLADOR DE PETICIONES (REQUEST HANDLER)
// ============================================================================
class UserRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        // Toda API REST debe responder explícitamente con su tipo de contenido
        response.setContentType("application/json");

        if (request.getMethod() == HTTPServerRequest::HTTP_GET) {
            manejarGet(request, response);
        } 
        else if (request.getMethod() == HTTPServerRequest::HTTP_POST) {
            manejarPost(request, response);
        } 
        else {
            // Manejo de verbos HTTP no implementados
            response.setStatus(HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
            std::ostream& ostr = response.send();
            ostr << "{\"error\": \"Método no permitido\"}" << std::endl;
        }
    }

private:
    void manejarGet(HTTPServerRequest& request, HTTPServerResponse& response) {
        try {
            Data::Session session("SQLite", "produccion.db");
            URI uri(request.getURI());
            auto queryParams = uri.getQueryParameters();
            
            int searchId = -1;
            for (const auto& param : queryParams) {
                if (param.first == "id") {
                    searchId = std::stoi(param.second);
                }
            }

            if (searchId == -1) {
                response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                std::ostream& ostr = response.send();
                ostr << "{\"error\": \"Falta el parámetro 'id' en la URL\"}" << std::endl;
                return;
            }

            Usuario u;
            Poco::Data::Statement select(session);
            select << "SELECT id, name, username, email FROM users WHERE id = ?",
                into(u.id), into(u.name), into(u.username), into(u.email), use(searchId);
            
            size_t rowsReturned = select.execute();

            if (rowsReturned == 0) {
                response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
                std::ostream& ostr = response.send();
                ostr << "{\"error\": \"Usuario no encontrado\"}" << std::endl;
                return;
            }

            // Construcción del JSON de respuesta
            JSON::Object jsonResponse;
            jsonResponse.set("id", u.id);
            jsonResponse.set("name", u.name);
            jsonResponse.set("username", u.username);
            jsonResponse.set("email", u.email);

            response.setStatus(HTTPResponse::HTTP_OK);
            std::ostream& ostr = response.send();
            jsonResponse.stringify(ostr);

        } catch (std::exception& e) {
            enviarErrorInterno(response, e.what());
        }
    }

    void manejarPost(HTTPServerRequest& request, HTTPServerResponse& response) {
        try {
            // Lectura directa del flujo de entrada del cuerpo HTTP
            std::istream& requestStream = request.stream();
            
            JSON::Parser parser;
            Dynamic::Var parseResult = parser.parse(requestStream);
            JSON::Object::Ptr jsonObject = parseResult.extract<JSON::Object::Ptr>();

            // Extracción de datos enviados por el cliente
            Usuario nuevoUsuario;
            nuevoUsuario.name = jsonObject->getValue<std::string>("name");
            nuevoUsuario.username = jsonObject->getValue<std::string>("username");
            nuevoUsuario.email = jsonObject->getValue<std::string>("email");

            // Persistencia en la Base de Datos SQLite
            Data::Session session("SQLite", "produccion.db");
            session << "INSERT INTO users (name, username, email) VALUES (?, ?, ?)",
                use(nuevoUsuario.name), use(nuevoUsuario.username), use(nuevoUsuario.email), now;

            // Obtener el ID autogenerado
            int nuevoId = 0;
            session << "SELECT last_insert_rowid()", into(nuevoId), now;

            // Respuesta formal REST (201 Created)
            JSON::Object jsonResponse;
            jsonResponse.set("id", nuevoId);
            jsonResponse.set("status", "Usuario creado con éxito");

            response.setStatus(HTTPResponse::HTTP_CREATED);
            std::ostream& ostr = response.send();
            jsonResponse.stringify(ostr);

        } catch (std::exception& e) {
            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
            std::ostream& ostr = response.send();
            ostr << "{\"error\": \"Payload JSON inválido: " << e.what() << "\"}" << std::endl;
        }
    }

    void enviarErrorInterno(HTTPServerResponse& response, const std::string& mensaje) {
        response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        std::ostream& ostr = response.send();
        ostr << "{\"error\": \"Error interno: " << mensaje << "\"}" << std::endl;
    }
};

// ============================================================================
// FÁBRICA DE ENRUTAMIENTO (ROUTING FACTORY)
// ============================================================================
class RestRouterFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
        URI uri(request.getURI());
        if (uri.getPath() == "/api/users") {
            return new UserRequestHandler;
        }
        return nullptr; // POCO responde automáticamente con 404 Not Found
    }
};

// ============================================================================
// BOOTSTRAP DE LA APLICACIÓN DE SERVIDOR
// ============================================================================
class RestServerApplication : public ServerApplication {
protected:
    int main(const std::vector<std::string>& /*args*/) override {
        // 1. Registro del Conector SQLite
        Data::SQLite::Connector::registerConnector();
        
        {
            // Inicialización de la base de datos local
            Data::Session session("SQLite", "produccion.db");
            session << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, username TEXT, email TEXT)", now;
            
            // Semilla de datos de prueba si la tabla está vacía
            int count = 0;
            session << "SELECT COUNT(*) FROM users", into(count), now;
            if (count == 0) {
                session << "INSERT INTO users (name, username, email) VALUES ('Linus Torvalds', 'linust', 'linus@linux.org')", now;
                session << "INSERT INTO users (name, username, email) VALUES ('Bjarne Stroustrup', 'bjarneb', 'bjarne@cpp.org')", now;
            }
        }

        // 2. Servidor HTTP
        unsigned short puerto = 8080;
        ServerSocket socket(puerto);
        HTTPServer servidor(new RestRouterFactory, socket, new HTTPServerParams);
        
        servidor.start();
        std::cout << "[INFO] Servidor REST de POCO levantado en el puerto " << puerto << std::endl;
        std::cout << "[INFO] Presione CTRL+C para apagar el backend." << std::endl;
        
        // Espera de señales del sistema operativo
        waitForTerminationRequest();
        
        // 3. Cierre limpio
        servidor.stop();
        Data::SQLite::Connector::unregisterConnector();
        std::cout << "[INFO] Servidor detenido de forma limpia." << std::endl;
        
        return Application::EXIT_OK;
    }
};

POCO_SERVER_MAIN(RestServerApplication)
