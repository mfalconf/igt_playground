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
    
    // Callback para cuando se REMUEVE un disco
    virtual void SetDriveRemovedCallback(std::function<void(const std::string&)> callback) = 0;

    // Callback para cuando se INSERTA un disco
    virtual void SetDriveAddedCallback(std::function<void(const std::string&)> callback) = 0;
};
