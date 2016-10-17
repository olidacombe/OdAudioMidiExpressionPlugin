#ifndef LEVELMETER_H_INCLUDED
#define LEVELMETER_H_INCLUDED

#include "JuceHeader.h"

class LevelMeter  : public Component {
public:

    LevelMeter ();
    ~LevelMeter();


    void paint (Graphics& g) override;
    void resized() override;



private:
    float level;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};

#endif   // LEVELMETER_H_INCLUDED
