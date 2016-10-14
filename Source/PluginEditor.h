#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class PluginProcessorEditor  : public AudioProcessorEditor,
                                                        private ComboBox::Listener
{
public:
    PluginProcessorEditor (PluginProcessor&, AudioProcessorValueTreeState&);
    ~PluginProcessorEditor();
    
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setMidiOutput(int index);
    void comboBoxChanged(ComboBox* box) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    ToggleButton thruButton;
    ScopedPointer<ButtonAttachment> thruAttachment;
    
    ComboBox midiOutputList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
