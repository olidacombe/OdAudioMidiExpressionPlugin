#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../JuceLibraryCode/JuceHeader.h"
int main(int argc, char* const argv[])
{
    // global setup...
    
    ScopedJuceInitialiser_GUI juce;

    int result = Catch::Session().run( argc, argv );

    // global clean-up...

    return result;

}
