#include "LinuxDrive.h"
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

LinuxDrive::LinuxDrive() : m_running(false), m_removedCallback(nullptr), m_addedCallback(nullptr) {}

LinuxDrive::~LinuxDrive() {
    StopMonitoring();
}

void LinuxDrive::StartMonitoring() {
    if (m_running) return;
    
    m_running = true;
    m_monitorThread = std::thread(&LinuxDrive::MonitorLoop, this);
}

void LinuxDrive::StopMonitoring() {
    if (!m_running) return;

    m_running = false;
    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
    }
}

void LinuxDrive::SetDriveRemovedCallback(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_removedCallback = callback;
}

void LinuxDrive::SetDriveAddedCallback(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(m_callbackMutex);
    m_addedCallback = callback;
}

void LinuxDrive::MonitorLoop() {
    // Initialize the general udev context
    struct udev* udevContext = udev_new();
    if (!udevContext) {
        std::cerr << "[ERROR] Failed to initialize the udev context." << std::endl;
        return;
    }

    // Create the monitor listening for 'udev' events from the Kernel Netlink socket
    struct udev_monitor* monitor = udev_monitor_new_from_netlink(udevContext, "udev");
    if (!monitor) {
        std::cerr << "[ERROR] Failed to create the udev monitor." << std::endl;
        udev_unref(udevContext);
        return;
    }

    // Filter only block-type devices (Disks, USBs, Partitions)
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "block", NULL);
    udev_monitor_enable_receiving(monitor);

    // Get the internal socket file descriptor to use with select()
    int fd = udev_monitor_get_fd(monitor);

    while (m_running) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        // 1-second timeout to prevent indefinite blocking if m_running changes to false
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(fd + 1, &fds, NULL, NULL, &timeout);
        
        // If data is ready on the socket and m_running is still active
        if (ret > 0 && FD_ISSET(fd, &fds) && m_running) {
            struct udev_device* dev = udev_monitor_receive_device(monitor);
            if (dev) {
                const char* action = udev_device_get_action(dev);
                
                if (action) {
                    std::string actionStr(action);
                    
                    // Only process "add" (insertion) or "remove" (extraction) events
                    if (actionStr == "add" || actionStr == "remove") {
                        const char* devnode = udev_device_get_devnode(dev);
                        std::string devicePath = devnode ? devnode : "Unknown Device";

                        // Try to extract additional properties (e.g., 'disk' or 'partition')
                        const char* devtype = udev_device_get_devtype(dev);
                        if (devtype) {
                            devicePath += " (" + std::string(devtype) + ")";
                        }

                        // Fire the corresponding callback in a thread-safe manner
                        std::lock_guard<std::mutex> lock(m_callbackMutex);
                        if (actionStr == "remove" && m_removedCallback) {
                            m_removedCallback(devicePath);
                        } else if (actionStr == "add" && m_addedCallback) {
                            m_addedCallback(devicePath);
                        }
                    }
                }
                udev_device_unref(dev);
            }
        }
    }

    // Orderly cleanup of resources when exiting the loop
    udev_monitor_unref(monitor);
    udev_unref(udevContext);
}
