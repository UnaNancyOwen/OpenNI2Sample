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
    while( !viewer.wasStopped() ){
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

    // Initialize Depth
    initializeDepth();

    // Initialize Point Cloud
    initializePointCloud();
}

// Initialize Device
inline void Device::initializeDevice()
{
    // Open Device
    OPENNI_CHECK( device.open( openni::ANY_DEVICE ) );
}

// Initialize Depth
inline void Device::initializeDepth()
{
    // Create Stream
    OPENNI_CHECK( depth_stream.create( device, openni::SENSOR_DEPTH ) );

    /*
    // Set Video Mode
    openni::VideoMode depth_mode;
    depth_mode.setResolution( depth_width, depth_height );
    depth_mode.setFps( depth_fps );
    depth_mode.setPixelFormat( openni::PixelFormat::PIXEL_FORMAT_DEPTH_1_MM );
    OPENNI_CHECK( depth_stream.setVideoMode( depth_mode ) );
    */

    // Start Stream
    OPENNI_CHECK( depth_stream.start() );
}

// Initialize Point Cloud
inline void Device::initializePointCloud()
{
    // Create Window
    viewer = cv::viz::Viz3d( "Point Cloud" );

    // Register Keyboard Callback Function
    viewer.registerKeyboardCallback( &keyboardCallback, this );

    // Show Coordinate System
    viewer.showWidget( "CoordinateSystem", cv::viz::WCameraPosition::WCameraPosition( 0.5 ) );
}

// Keyboard Callback Function
void Device::keyboardCallback( const cv::viz::KeyboardEvent& event, void* cookie )
{
    // Exit Viewer when Pressed ESC key
    if( event.code == 'q' && event.action == cv::viz::KeyboardEvent::Action::KEY_DOWN ){

        // Retrieve Viewer
        cv::viz::Viz3d viewer = static_cast<Device*>( cookie )->viewer;

        // Close Viewer
        viewer.close();
    }
    // Save Point Cloud to File when Pressed 's' key
    else if( event.code == 's' && event.action == cv::viz::KeyboardEvent::Action::KEY_DOWN ){
        // Retrieve Point Cloud and Color
        cv::Mat cloud = static_cast<Device*>( cookie )->vertices_mat;

        // Generate File Name
        static uint8_t i = 0;
        std::ostringstream oss;
        oss << std::setfill( '0' ) << std::setw( 3 ) << i++;
        std::string file = oss.str() + ".ply";

        // Write Point Cloud to File
        cv::viz::writeCloud( file, cloud, cv::noArray(), false );
    }
};

// Finalize
void Device::finalize()
{
    // Close Windows
    cv::destroyAllWindows();
}

// Update Data
void Device::update()
{
    // Update Depth
    updateDepth();
}

// Update Depth
inline void Device::updateDepth()
{
    // Update Frame
    OPENNI_CHECK( depth_stream.readFrame( &depth_frame ) );

    // Retrive Frame Size
    depth_width = depth_frame.getWidth();
    depth_height = depth_frame.getHeight();
}

// Draw Data
void Device::draw()
{
    // Draw Point Cloud
    drawPointCloud();
}

// Draw Point Cloud
inline void Device::drawPointCloud()
{
    if( !depth_frame.isValid() ){
        return;
    }

    // Retrieve Depth
    const uint16_t* depth = static_cast<const uint16_t*>( depth_frame.getData() );

    // Create cv::Mat from Vertices and Texture
    vertices_mat = cv::Mat( depth_height, depth_width, CV_32FC3, cv::Vec3f::all( std::numeric_limits<float>::quiet_NaN() ) );

    #pragma omp parallel for
    for( uint32_t y = 0; y < depth_height; y++ ){
        for( uint32_t x = 0; x < depth_width; x++ ){
            // Retrieve Depth
            const uint16_t z = depth[y * depth_width + x];
            if( !z ){
                continue;
            }

            // Convert Depth to World
            float wx, wy, wz;
            OPENNI_CHECK( openni::CoordinateConverter::convertDepthToWorld( depth_stream, static_cast<float>( x ), static_cast<float>( y ), static_cast<float>( z ), &wx, &wy, &wz ) );

            // Add Point to Vertices
            vertices_mat.at<cv::Vec3f>( y, x ) = cv::Vec3f( wx, wy, wz );
        }
    }
}

// Show Data
void Device::show()
{
    // Show Point Cloud
    showPointCloud();
}

// Show Point Cloud
inline void Device::showPointCloud()
{
    if( vertices_mat.empty() ){
        return;
    }

    // Create Point Cloud
    cv::viz::WCloud cloud( vertices_mat, cv::viz::Color::white() );

    // Show Point Cloud
    viewer.showWidget( "Cloud", cloud );
    viewer.spinOnce();
}
