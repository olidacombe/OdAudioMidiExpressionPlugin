#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class MidiOutputComboBox : public ComboBox
{
public:
    MidiOutputComboBox(MidiOutputList& mol) : midiOutputList(mol)
    {}
private:
    MidiOutputList& midiOutputList;
};

class PluginProcessorEditor  : public AudioProcessorEditor,
                                private ComboBox::Listener,
                                public ChangeListener
{
public:
    PluginProcessorEditor (PluginProcessor&, AudioProcessorValueTreeState&);
    ~PluginProcessorEditor();
    
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setMidiOutput(int index);
    void comboBoxChanged(ComboBox* box) override;
    void changeListenerCallback(ChangeBroadcaster* src) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    Label decayLabel;
    Slider decaySlider;
    ScopedPointer<SliderAttachment> decayAttachment;
    
    ToggleButton thruButton;
    ScopedPointer<ButtonAttachment> thruAttachment;
    
    ToggleButton activeButton;
    ScopedPointer<ButtonAttachment> activeAttachment;
    
    ComboBox midiOutputList;

    void updateAvailableMidiOutputList();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
