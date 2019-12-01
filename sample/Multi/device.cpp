#include "device.h"
#include "util.h"

// Constructor
Device::Device( const std::string& device_name, const std::string& device_uri )
    : device_name( device_name ),
      device_uri( device_uri )
{
    // Initialize
    initialize();
}

// Destructor
Device::~Device()
{
    // Finalize
    finalize();
}

// Initialize
void Device::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Device
    initializeDevice();

    // Initialize Color
    initializeColor();
}

// Initialize Device
inline void Device::initializeDevice()
{
    // Open Device
    OPENNI_CHECK( device.open( device_uri.c_str() ) );
}

// Initialize Color
inline void Device::initializeColor()
{
    // Create Stream
    OPENNI_CHECK( color_stream.create( device, openni::SENSOR_COLOR ) );

    /*
    // Set Video Mode
    openni::VideoMode color_mode;
    color_mode.setResolution( color_width, color_height );
    color_mode.setFps( color_fps );
    color_mode.setPixelFormat( openni::PixelFormat::PIXEL_FORMAT_RGB888 );
    OPENNI_CHECK( color_stream.setVideoMode( color_mode ) );
    */

    // Start Stream
    OPENNI_CHECK( color_stream.start() );
}

// Finalize
void Device::finalize()
{
    // Close Windows
    cv::destroyAllWindows();
}

// Update Data
void Device::update()
{
    // Update Color
    updateColor();
}

// Update Color
inline void Device::updateColor()
{
    // Update Frame
    OPENNI_CHECK( color_stream.readFrame( &color_frame ) );

    // Retrive Frame Size
    color_width = color_frame.getWidth();
    color_height = color_frame.getHeight();
}

// Draw Data
void Device::draw()
{
    // Draw Color
    drawColor();
}

// Draw Color
inline void Device::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.getData() ) );

    // Convert RGB to BGR
    cv::cvtColor( color_mat, color_mat, cv::COLOR_RGB2BGR );
}

// Show Data
void Device::show()
{
    // Show Color
    showColor();
}

// Show Color
inline void Device::showColor()
{
    if( color_mat.empty() ){
        return;
    }

    // Show Color Image
    cv::imshow( "Color - " + device_name + " (" + device_uri + ")", color_mat );
}
