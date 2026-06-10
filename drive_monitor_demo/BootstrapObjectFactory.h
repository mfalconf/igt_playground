#pragma once
#include "IDrive.h"

#if defined(__linux__)
#include "LinuxDrive.h"
#elif defined(_WIN32)
#include "WindowsDrive.h" // Tu clase WMI original
#endif

class BootstrapObjectFactory {
public:
    static IDrive* CreateDriveMonitor() {
#if defined(__linux__)
        return new LinuxDrive();
#elif defined(_WIN32)
        // Aquí iría el retorno de la implementación clásica de Windows (WMI)
        #error "Plataforma no soportada en esta demo"
        return nullptr;
#endif
    }
};
