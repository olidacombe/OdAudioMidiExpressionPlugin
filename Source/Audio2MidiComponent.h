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

    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    // redundancy?  MidiOutWorker could use access to a AudioProcessorValueTreeState
    // for its settings.  And the state could be retrieved from there.
    Audio2MidiComponent(SubProcessor&);
    virtual ~Audio2MidiComponent()=0;
    virtual void paint(Graphics&) override;

protected:
    AudioProcessorValueTreeState& parameters;
    ScopedPointer<MidiOutputComboBox> midiOutputComboBox;
    ScopedPointer<LevelMeter> levelMeter;
    
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Audio2MidiComponent)
};


class LoudnessDecayComponent : public Audio2MidiComponent
{
public:
    LoudnessDecayComponent(SubProcessor&);
    ~LoudnessDecayComponent();
    void paint (Graphics&) override;
    void resized() override;

private:
    Label decayLabel;
    Slider decaySlider;
    ScopedPointer<SliderAttachment> decayAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessDecayComponent)
};


#endif  // AUDIO2MIDICOMPONENT_H_INCLUDED
