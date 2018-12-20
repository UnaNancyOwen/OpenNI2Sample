cmake_minimum_required( VERSION 3.6 )

# Require C++11 (or later)
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# Create Project
project( Sample )
add_executable( Multi multidevice.h multidevice.cpp device.h device.cpp util.h main.cpp )

# Set StartUp Project
set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "Multi" )

# Find Package
# OpenNI2
set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH} )
find_package( OpenNI2 REQUIRED )

# OpenCV
set( OpenCV_DIR "C:/Program Files/opencv/build" CACHE PATH "Path to OpenCV config directory." )
find_package( OpenCV REQUIRED )

if( OpenNI2_FOUND AND OpenCV_FOUND )
  # Additional Include Directories
  include_directories( ${OpenNI2_INCLUDE_DIR} )
  include_directories( ${OpenCV_INCLUDE_DIRS} )

  # Additional Dependencies
  target_link_libraries( Multi ${OpenNI2_LIBRARY} )
  target_link_libraries( Multi ${OpenCV_LIBS} )
endif()
