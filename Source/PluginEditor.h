#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Audio2MidiComponent.h"

class PluginProcessorEditor  : public AudioProcessorEditor
{
public:
    PluginProcessorEditor (PluginProcessor&, AudioProcessorValueTreeState&);
    ~PluginProcessorEditor();
    
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    Audio2MidiComponent* subProcessorComponent;
    
    OwnedArray<Audio2MidiComponent> audio2MidiComponents;
    
    ToggleButton thruButton;
    ScopedPointer<ButtonAttachment> thruAttachment;
    
    ToggleButton activeButton;
    ScopedPointer<ButtonAttachment> activeAttachment;

    void updateAvailableMidiOutputList();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
