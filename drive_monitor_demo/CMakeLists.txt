#cmake_minimum_required(VERSION 3.10)
#project(DriveMonitorDemo CXX)

# Forzar el estándar C++11 o superior para soporte nativo de hilos y lambdas
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurar el ejecutable
add_executable(run_drive_monitor_demo 
    main.cpp 
    LinuxDrive.cpp 
    LinuxDrive.h 
    IDrive.h 
    BootstrapObjectFactory.h
)

# Buscar y enlazar la librería libudev del sistema operativo
find_library(UDEV_LIB udev REQUIRED)

# Enlazar la librería udev y soporte de hilos POSIX (pthread)
target_link_libraries(run_drive_monitor_demo ${UDEV_LIB} pthread)