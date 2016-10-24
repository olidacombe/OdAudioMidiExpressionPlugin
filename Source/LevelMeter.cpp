#include "LevelMeter.h"


LevelMeter::LevelMeter ()
{
    
    setSize (5, 50);

}

LevelMeter::~LevelMeter()
{

}

//==============================================================================
void LevelMeter::paint (Graphics& g)
{

    g.fillAll (Colours::black);
    g.setColour(Colours::yellow);
    Rectangle<int> area = getLocalBounds();
    g.drawRect(area.removeFromBottom(getHeight() * 0.7));

}

void LevelMeter::resized()
{

}

