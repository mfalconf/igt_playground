# Configure the executable
add_executable(run_drive_monitor_demo 
    main.cpp 
    LinuxDrive.cpp 
    LinuxDrive.h 
    IDrive.h 
    BootstrapObjectFactory.h
)

# Find and link the operating system's libudev library
find_library(UDEV_LIB udev REQUIRED)

# Link the udev library and POSIX threads support (pthread)
target_link_libraries(run_drive_monitor_demo ${UDEV_LIB} pthread)
