#ifndef __DEVICE__
#define __DEVICE__

#include <OpenNI.h>
#include <opencv2/opencv.hpp>

class Device
{
private:
    // Device
    openni::Device device;
    openni::VideoStream color_stream;
    std::string device_name;
    std::string device_uri;

    // Color Buffer
    openni::VideoFrameRef color_frame;
    cv::Mat color_mat;
    uint32_t color_width = 640;
    uint32_t color_height = 480;
    uint32_t color_fps = 30;

public:
    // Constructor
    Device( const std::string& device_name, const std::string& device_uri );

    // Destructor
    ~Device();

    // Update Data
    void update();

    // Draw Data
    void draw();

    // Show Data
    void show();

private:
    // Initialize
    void initialize();

    // Initialize Device
    inline void initializeDevice();

    // Initialize Color
    inline void initializeColor();

    // Finalize
    void finalize();

    // Update Color
    inline void updateColor();

    // Draw Color
    inline void drawColor();

    // Show Color
    inline void showColor();
};

#endif // __DEVICE__
