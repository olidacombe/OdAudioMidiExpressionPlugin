#include "LevelMeter.h"


LevelMeter::LevelMeter ()
{
    
    setSize (20, 40);

}

LevelMeter::~LevelMeter()
{

}

//==============================================================================
void LevelMeter::paint (Graphics& g)
{

    g.fillAll (Colours::black);
    g.setColour(Colours::yellow);
    Rectangle<int> area = getBounds();
    g.drawRect(area.removeFromBottom(getHeight() * 0.7));

}

void LevelMeter::resized()
{

}

