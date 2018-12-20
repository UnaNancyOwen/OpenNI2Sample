#ifndef __MULTIDEVICE__
#define __MULTIDEVICE__

#include "device.h"

#include <OpenNI.h>
#include <opencv2/opencv.hpp>

#include <vector>
#include <memory>

class MultiDevice
{
private:
    // RealSense
    std::vector<std::unique_ptr<Device>> devices;

public:
    // Constructor
    MultiDevice();

    // Destructor
    ~MultiDevice();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeDevice( const openni::DeviceInfo& device_info );

    // Finalize
    void finalize();
};

#endif // __MULTIDEVICE__
