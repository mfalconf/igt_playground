#pragma once
#include "IDrive.h"
#include <libudev.h>
#include <thread>
#include <atomic>
#include <mutex>

class LinuxDrive : public IDrive {
public:
    LinuxDrive();
    ~LinuxDrive() override;

    void StartMonitoring() override;
    void StopMonitoring() override;
    
    void SetDriveRemovedCallback(std::function<void(const std::string&)> callback) override;
    void SetDriveAddedCallback(std::function<void(const std::string&)> callback) override;

private:
    void MonitorLoop();

    std::thread m_monitorThread;
    std::atomic<bool> m_running;
    
    std::function<void(const std::string&)> m_removedCallback;
    std::function<void(const std::string&)> m_addedCallback;
    std::mutex m_callbackMutex;
};
