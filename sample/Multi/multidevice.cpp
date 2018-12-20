#include "multidevice.h"
#include "util.h"

// Constructor
MultiDevice::MultiDevice()
{
    // Initialize
    initialize();
}

// Destructor
MultiDevice::~MultiDevice()
{
    // Finalize
    finalize();
}

// Processing
void MultiDevice::run()
{
    // Main Loop
    while( true ){
        for( std::unique_ptr<Device>& device : devices ){
            // Update Data
            device->update();

            // Draw Data
            device->draw();

            // Show Data
            device->show();
        }

        // Key Check
        const int32_t key = cv::waitKey( 33 );
        if( key == 'q' ){
            break;
        }
    }
}

// Initialize
void MultiDevice::initialize()
{
    cv::setUseOptimized( true );

    // Initialize OpenNI2
    OPENNI_CHECK( openni::OpenNI::initialize() );

    // Retrive Connected Devices List
    openni::Array<openni::DeviceInfo> device_info_list;
    openni::OpenNI::enumerateDevices( &device_info_list );

    // Initialize Connected Devices
    for( uint32_t i = 0; i < device_info_list.getSize(); i++ ){
        const openni::DeviceInfo& device_info = device_info_list[i];

        // Initialize Device
        initializeDevice( device_info );
    }
}

// Initialize Device
inline void MultiDevice::initializeDevice( const openni::DeviceInfo& device_info )
{
    // Retrieve Device URI
    const std::string device_name = device_info.getName();
    const std::string device_uri = device_info.getUri();

    // Add Device to Container
    devices.push_back( std::make_unique<Device>( device_name, device_uri ) );
}

// Finalize
void MultiDevice::finalize()
{
    // Close Windows
    cv::destroyAllWindows();
}
