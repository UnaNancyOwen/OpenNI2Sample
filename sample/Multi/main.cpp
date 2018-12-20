#include <iostream>
#include <sstream>

#include "multidevice.h"

int main( int argc, char* argv[] )
{
    try{
        MultiDevice multidevice;
        multidevice.run();
    } catch( std::exception& ex ){
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
