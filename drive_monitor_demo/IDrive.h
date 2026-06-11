#pragma once
#include <string>
#include <functional>

// Pure interface defining the behavior of the drive monitor
class IDrive {
public:
    virtual ~IDrive() = default;
    
    // Starts the background monitoring thread
    virtual void StartMonitoring() = 0;
    
    // Stops the thread and releases resources safely
    virtual void StopMonitoring() = 0;
    
    // Callback triggered when a drive is REMOVED
    virtual void SetDriveRemovedCallback(std::function<void(const std::string&)> callback) = 0;

    // Callback triggered when a drive is INSERTED
    virtual void SetDriveAddedCallback(std::function<void(const std::string&)> callback) = 0;
};
