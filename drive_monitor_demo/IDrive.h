#pragma once
#include <string>
#include <functional>

// Interfaz pura que define el comportamiento del monitor de discos
class IDrive {
public:
    virtual ~IDrive() = default;
    
    // Inicia el hilo de monitoreo en segundo plano
    virtual void StartMonitoring() = 0;
    
    // Detiene el hilo y libera los recursos de forma segura
    virtual void StopMonitoring() = 0;
    
    // Registra la función callback que se ejecutará cuando se remueva un disco
    virtual void SetDriveRemovedCallback(std::function<void(const std::string&)> callback) = 0;
};
