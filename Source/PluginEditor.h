/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class OdAudioMidiExpressionPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                                        private ComboBox::Listener
{
public:
    OdAudioMidiExpressionPluginAudioProcessorEditor (OdAudioMidiExpressionPluginAudioProcessor&);
    ~OdAudioMidiExpressionPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void setMidiOutput(int index);
    void comboBoxChanged(ComboBox* box) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OdAudioMidiExpressionPluginAudioProcessor& processor;
    ComboBox midiOutputList;
    ScopedPointer<MidiOutput> midiOutput;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OdAudioMidiExpressionPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
