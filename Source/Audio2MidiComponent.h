/*
  ==============================================================================

    Audio2MidiComponent.h
    Created: 18 Oct 2016 5:48:31pm
    Author:  Oli

  ==============================================================================
*/

#ifndef AUDIO2MIDICOMPONENT_H_INCLUDED
#define AUDIO2MIDICOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "LevelMeter.h"
//#include "MidiOutWorker.h"

//==============================================================================
/*
*/

class MidiOutWorker;

class MidiOutputComboBox : public ComboBox
{
public:
    MidiOutputComboBox(MidiOutWorker& mow) : midiOutputWorker(mow)
    {}
private:
    MidiOutWorker& midiOutputWorker;
};

class LevelMeter;

class Audio2MidiComponent    : public Component
{
public:
    Audio2MidiComponent();
    virtual ~Audio2MidiComponent()=0;
    virtual void paint(Graphics&) override;

protected:
    ScopedPointer<LevelMeter> levelMeter;

private:
    ScopedPointer<MidiOutputComboBox> midiOutputComboBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Audio2MidiComponent)
};


class LoudnessDecayComponent : public Audio2MidiComponent
{
public:
    LoudnessDecayComponent();
    ~LoudnessDecayComponent();
    void paint (Graphics&) override;
    void resized() override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessDecayComponent)
};


#endif  // AUDIO2MIDICOMPONENT_H_INCLUDED
