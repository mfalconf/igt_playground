#include "LinuxDrive.h"
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

LinuxDrive::LinuxDrive() : m_running(false), m_callback(nullptr) {}

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
    m_callback = callback;
}

void LinuxDrive::MonitorLoop() {
    // 1. Inicializar el contexto general de udev
    struct udev* udevContext = udev_new();
    if (!udevContext) {
        std::cerr << "[ERROR] No se pudo inicializar el contexto udev." << std::endl;
        return;
    }

    // 2. Crear el monitor escuchando eventos 'udev' desde el Netlink del Kernel
    struct udev_monitor* monitor = udev_monitor_new_from_netlink(udevContext, "udev");
    if (!monitor) {
        std::cerr << "[ERROR] No se pudo crear el monitor udev." << std::endl;
        udev_unref(udevContext);
        return;
    }

    // 3. Filtrar solo por dispositivos de tipo de bloque (Discos, USBs, Particiones)
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "block", NULL);
    udev_monitor_enable_receiving(monitor);

    // Obtener el File Descriptor interno del socket para usarlo con select()
    int fd = udev_monitor_get_fd(monitor);

    while (m_running) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        
        // Timeout de 1 segundo para evitar bloqueos indefinidos si m_running cambia a false
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ret = select(fd + 1, &fds, NULL, NULL, &timeout);
        
        // Si hay datos listos en el socket y m_running sigue activo
        if (ret > 0 && FD_ISSET(fd, &fds) && m_running) {
            struct udev_device* dev = udev_monitor_receive_device(monitor);
            if (dev) {
                const char* action = udev_device_get_action(dev);
                
                // Verificar si la acción del Kernel es una remoción
                if (action && std::string(action) == "remove") {
                    const char* devnode = udev_device_get_devnode(dev);
                    std::string devicePath = devnode ? devnode : "Dispositivo Desconocido";

                    // Opcional: Intentar extraer propiedades adicionales heredadas antes de la destrucción
                    const char* devtype = udev_device_get_devtype(dev);
                    if (devtype) {
                        devicePath += " (" + std::string(devtype) + ")";
                    }

                    // Disparar el callback de forma segura
                    std::lock_guard<std::mutex> lock(m_callbackMutex);
                    if (m_callback) {
                        m_callback(devicePath);
                    }
                }
                udev_device_unref(dev);
            }
        }
    }

    // 4. Limpieza ordenada de recursos al salir del bucle
    udev_monitor_unref(monitor);
    udev_unref(udevContext);
}
