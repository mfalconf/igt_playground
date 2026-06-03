#include <sp.h>
#include <iostream>

int main() {
    char user[] = "mi_app_c++";
    char spread_name[] = "4803@localhost"; // Puerto por defecto
    mailbox mbox;
    char private_group[MAX_GROUP_NAME];

    // Conectar al demonio de Spread
    int ret = SP_connect(spread_name, user, 0, 1, &mbox, private_group);
    
    if (ret < 0) {
        std::cerr << "Error al conectar a Spread: " << ret << std::endl;
        return 1;
    }
    
    std::cout << "Conectado. Mi grupo privado es: " << private_group << std::endl;

    // Unirse a un grupo público
    char group_name[] = "grupo_chat";
    ret = SP_join(mbox, group_name);
    if (ret < 0) {
        std::cerr << "Error al unirse al grupo." << std::endl;
    } else {
        std::cout << "Unido exitosamente a " << group_name << std::endl;
    }

    // Desconectar
    SP_disconnect(mbox);
    return 0;
}
