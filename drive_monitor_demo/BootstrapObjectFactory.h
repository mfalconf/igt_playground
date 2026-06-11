#pragma once
#include "IDrive.h"

#if defined(__linux__)
#include "LinuxDrive.h"
#elif defined(_WIN32)
#include "WindowsDrive.h" // Your original WMI class
#endif

class BootstrapObjectFactory {
public:
    static IDrive* CreateDriveMonitor() {
#if defined(__linux__)
        return new LinuxDrive();
#elif defined(_WIN32)
        // Here would go the return of the classic Windows implementation (WMI)
        #error "Platform not supported in this demo"
        return nullptr;
#endif
    }
};
