#include "BootstrapObjectFactory.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=========================================" << std::endl;
    #if defined(__linux__)
    std::cout << "  Iniciando Demo de Monitoreo en LINUX   " << std::endl;
    #endif
    std::cout << "=========================================" << std::endl;

    // Crear la instancia del monitor usando la fábrica integrada
    std::unique_ptr<IDrive> driveMonitor(BootstrapObjectFactory::CreateDriveMonitor());

    if (!driveMonitor) {
        std::cerr << "Error crítico: No se pudo crear el monitor de discos." << std::endl;
        return 1;
    }

    // 1. Registrar el callback para REMOCIÓN (extracción)
    driveMonitor->SetDriveRemovedCallback([](const std::string& drivePath) {
        std::cout << "\n[ - REMOVIDO ] Se desconectó la unidad:" << std::endl;
        std::cout << "[DETALLE]: " << drivePath << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    });

    // 2. Registrar el callback para INSERCIÓN
    driveMonitor->SetDriveAddedCallback([](const std::string& drivePath) {
        std::cout << "\n[ + INSERTADO ] Se detectó una nueva unidad:" << std::endl;
        std::cout << "[DETALLE]: " << drivePath << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    });

    // Iniciar el monitoreo asíncrono
    std::cout << "[INFO] Activando el monitor udev..." << std::endl;
    driveMonitor->StartMonitoring();
    std::cout << "[OK] Buscando eventos del Kernel (Inserción/Extracción) en segundo plano..." << std::endl;
    std::cout << "[INFO] Presiona ENTER en cualquier momento para salir de la demo.\n" << std::endl;

    // Bloquear el hilo de ejecución principal esperando la interacción del usuario
    std::cin.get();

    // Detener de forma controlada el hilo secundario
    std::cout << "[INFO] Deteniendo servicios de monitoreo de hardware..." << std::endl;
    driveMonitor->StopMonitoring();
    std::cout << "[OK] Demo finalizada con éxito." << std::endl;

    return 0;
}
