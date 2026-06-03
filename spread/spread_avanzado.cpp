#include <sp.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

int main() {
    char user[] = "nodo_c++";
    char spread_name[] = "4803@localhost"; 
    mailbox mbox;
    char private_group[MAX_GROUP_NAME];
    char group_name[] = "grupo_operaciones";

    // 1. Conexión
    int ret = SP_connect(spread_name, user, 0, 1, &mbox, private_group);
    if (ret < 0) {
        std::cerr << "Error al conectar: " << ret << std::endl;
        return 1;
    }
    
    // 2. Unirse al grupo
    SP_join(mbox, group_name);
    std::cout << "[INFO] Unido al grupo: " << group_name << std::endl;

    // 3. Enviar un mensaje (Multicast)
    char mensaje_enviar[] = "¡Hola a todos desde C++!";
    
    // Enviamos usando la garantía AGREED_MESS
    ret = SP_multicast(mbox, AGREED_MESS, group_name, 1, sizeof(mensaje_enviar), mensaje_enviar);
    if (ret < 0) {
        std::cerr << "Error al enviar mensaje" << std::endl;
    } else {
        std::cout << "[INFO] Mensaje enviado." << std::endl;
    }

    // 4. Bucle de Recepción
    std::cout << "[INFO] Esperando mensajes..." << std::endl;
    
    while (true) {
        service service_type;
        char sender[MAX_GROUP_NAME];
        int num_groups;
        char target_groups[10][MAX_GROUP_NAME]; // Máximo 10 grupos de destino
        int16 mess_type;
        int endian_mismatch;
        char payload[2048]; // Buffer para el mensaje entrante

        // Esta función bloquea el hilo hasta que llega un mensaje
        ret = SP_receive(mbox, &service_type, sender, 10, &num_groups, 
                         target_groups, &mess_type, &endian_mismatch, 
                         sizeof(payload), payload);

        if (ret < 0) {
            std::cerr << "Error al recibir o conexión perdida." << std::endl;
            break;
        }

        // Determinar qué tipo de mensaje llegó
        if (Is_regular_mess(service_type)) {
            // Asegurarnos de que el string termine en nulo para imprimirlo
            payload[ret] = '\0'; 
            std::cout << "[MENSAJE REGULAR] De: " << sender 
                      << " | Contenido: " << payload << std::endl;
        } 
        else if (Is_membership_mess(service_type)) {
            std::cout << "[MEMBRESÍA] El grupo ha cambiado. " 
                      << "Alguien entró o salió (Emisor del aviso: " << sender << ")" << std::endl;
        }
    }

    // 5. Desconexión (Aunque por el while(true) no llegará aquí a menos que haya error)
    SP_disconnect(mbox);
    return 0;
}
