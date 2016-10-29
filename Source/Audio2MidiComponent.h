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
#include "SubProcessor.h"
#include "LevelMeter.h"
//#include "MidiOutWorker.h"

//==============================================================================
/*
*/

class MidiOutWorker;
class SubProcessor;

class MidiOutputComboBox : public ComboBox, public ChangeListener
{
public:
    MidiOutputComboBox(MidiOutWorker* mow);
    void changeListenerCallback(ChangeBroadcaster *);
private:
    void refreshList();

    MidiOutWorker* midiOutputWorker;
    SharedResourcePointer<MidiOutputList> midiOutputList;
};


class Audio2MidiComponent    : public Component
{
public:

    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    Audio2MidiComponent(SubProcessor&);
    virtual ~Audio2MidiComponent();
    virtual void paint(Graphics&) override;

protected:
    SubProcessor& subProcessor;
    AudioProcessorValueTreeState& parameters;

    ScopedPointer<MidiOutputComboBox> midiOutputComboBox;
    ScopedPointer<LevelMeter> levelMeter;
    
    String localizedParameterID(const String& parameterID);
    
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
