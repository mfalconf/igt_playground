#include "BootstrapObjectFactory.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=========================================" << std::endl;
    #if defined(__linux__)
    std::cout << "    Linux Drive Monitor Demo Starting    " << std::endl;
    #endif
    std::cout << "=========================================" << std::endl;

    // Create the monitor instance using the built-in factory
    std::unique_ptr<IDrive> driveMonitor(BootstrapObjectFactory::CreateDriveMonitor());

    if (!driveMonitor) {
        std::cerr << "Critical error: Failed to create the drive monitor." << std::endl;
        return 1;
    }

    // 1. Register callback for REMOVAL (disconnection)
    driveMonitor->SetDriveRemovedCallback([](const std::string& drivePath) {
        std::cout << "\n[ - REMOVED ] Drive disconnected:" << std::endl;
        std::cout << "[DETAIL]: " << drivePath << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    });

    // 2. Register callback for INSERTION
    driveMonitor->SetDriveAddedCallback([](const std::string& drivePath) {
        std::cout << "\n[ + INSERTED ] New drive detected:" << std::endl;
        std::cout << "[DETAIL]: " << drivePath << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    });

    // Start asynchronous monitoring
    std::cout << "[INFO] Activating udev monitor..." << std::endl;
    driveMonitor->StartMonitoring();
    std::cout << "[OK] Listening for Kernel events (Insertion/Removal) in the background..." << std::endl;
    std::cout << "[INFO] Press ENTER at any time to exit the demo." << std::endl;

    // Block the main thread waiting for user input
    std::cin.get();

    // Gracefully stop the background thread
    std::cout << "[INFO] Stopping hardware monitoring services..." << std::endl;
    driveMonitor->StopMonitoring();
    std::cout << "[OK] Demo finished successfully." << std::endl;

    return 0;
}
