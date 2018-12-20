#ifndef __DEVICE__
#define __DEVICE__

#include <OpenNI.h>
#include <opencv2/opencv.hpp>

class Device
{
private:
    // Device
    openni::Device device;
    openni::VideoStream infrared_stream;

    // Infrared Buffer
    openni::VideoFrameRef infrared_frame;
    cv::Mat infrared_mat;
    uint32_t infrared_width = 640;
    uint32_t infrared_height = 480;
    uint32_t infrared_fps = 30;

public:
    // Constructor
    Device();

    // Destructor
    ~Device();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Device
    inline void initializeDevice();

    // Initialize Infrared
    inline void initializeInfrared();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Infrared
    inline void updateInfrared();

    // Draw Data
    void draw();

    // Draw Infrared
    inline void drawInfrared();

    // Show Data
    void show();

    // Show Infrared
    inline void showInfrared();
};

#endif // __DEVICE__
