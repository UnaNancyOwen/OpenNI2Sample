#include "device.h"
#include "util.h"

// Constructor
Device::Device()
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

// Processing
void Device::run()
{
    // Main Loop
    while( true ){
        // Update Data
        update();

        // Draw Data
        draw();

        // Show Data
        show();

        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
    }
}

// Initialize
void Device::initialize()
{
    cv::setUseOptimized( true );

    // Initialize OpenNI2
    OPENNI_CHECK( openni::OpenNI::initialize() );

    // Initialize Device
    initializeDevice();

    // Initialize Infrared
    initializeInfrared();
}

// Initialize Device
inline void Device::initializeDevice()
{
    // Open Device
    OPENNI_CHECK( device.open( openni::ANY_DEVICE ) );
}

// Initialize Infrared
inline void Device::initializeInfrared()
{
    // Create Stream
    OPENNI_CHECK( infrared_stream.create( device, openni::SENSOR_IR ) );

    /*
    // Set Video Mode
    openni::VideoMode infrared_mode = infrared_stream.getVideoMode();
    infrared_mode.setResolution( infrared_width, infrared_height );
    infrared_mode.setFps( infrared_fps );
    //infrared_mode.setPixelFormat( openni::PixelFormat::PIXEL_FORMAT_GRAY16 );
    //infrared_mode.setPixelFormat( openni::PixelFormat::PIXEL_FORMAT_GRAY8 );
    OPENNI_CHECK( infrared_stream.setVideoMode( infrared_mode ) );
    */

    // Start Stream
    OPENNI_CHECK( infrared_stream.start() );
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
    // Update Infrared
    updateInfrared();
}

// Update Infrared
inline void Device::updateInfrared()
{
    // Update Frame
    OPENNI_CHECK( infrared_stream.readFrame( &infrared_frame ) );

    // Retrive Frame Size
    infrared_width = infrared_frame.getWidth();
    infrared_height = infrared_frame.getHeight();
}

// Draw Data
void Device::draw()
{
    // Draw Infrared
    drawInfrared();
}

// Draw Infrared
inline void Device::drawInfrared()
{
    // Create cv::Mat form Infrared Frame
    const openni::PixelFormat pixel_format = infrared_frame.getVideoMode().getPixelFormat();
    switch( pixel_format ){
        case openni::PixelFormat::PIXEL_FORMAT_GRAY16:
            infrared_mat = cv::Mat( infrared_height, infrared_width, CV_16UC1, const_cast<void*>( infrared_frame.getData() ) );
            break;
        case openni::PixelFormat::PIXEL_FORMAT_GRAY8:
            infrared_mat = cv::Mat( infrared_height, infrared_width, CV_8UC1, const_cast<void*>( infrared_frame.getData() ) );
            break;
        default:
            throw std::runtime_error( "failed can not support this format" );
            std::exit( EXIT_FAILURE );
    }
}

// Show Data
void Device::show()
{
    // Show Infrared
    showInfrared();
}

// Show Infrared
inline void Device::showInfrared()
{
    if( infrared_mat.empty() ){
        return;
    }

    // Scaling ( 0b1111'1111'0000'0000 -> 0b1111'1111 )
    cv::Mat scale_mat( infrared_height, infrared_width, CV_8UC1 );
    if( infrared_mat.depth() == CV_16U ){
        scale_mat.forEach<uint8_t>( [&]( uint8_t &p, const int32_t* position ){
            p = infrared_mat.at<uint16_t>( position[0], position[1] ) >> 8;
        } );
    }
    else{
        scale_mat = infrared_mat;
    }

    // Show Infrared Image
    cv::imshow( "Infrared", scale_mat );
}
