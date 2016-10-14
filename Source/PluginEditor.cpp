#include "PluginEditor.h"


//==============================================================================
PluginProcessorEditor::PluginProcessorEditor (PluginProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    
    thruButton.setButtonText ("Thru");
    addAndMakeVisible (thruButton);
    thruAttachment = new ButtonAttachment (valueTreeState, "thru", thruButton);
    
    
    addAndMakeVisible(midiOutputList);
    midiOutputList.setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
    const StringArray midiOutputs(MidiOutput::getDevices());
    midiOutputList.addItemList(midiOutputs, 1);
    midiOutputList.addListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

PluginProcessorEditor::~PluginProcessorEditor()
{
    //midiOutput = nullptr;
}

//==============================================================================
void PluginProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void PluginProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    thruButton.setBounds(area.removeFromTop(20));
    midiOutputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
}

void PluginProcessorEditor::setMidiOutput(int index) {
    //midiOutput = MidiOutput::openDevice(index);
    //if(midiOutput != nullptr) {
    //    midiOutputList.setSelectedId(index + 1, dontSendNotification);
    //    processor.setMidiOutput(index);
    //}
    midiOutputList.setSelectedId(processor.setMidiOutput(index));
}

void PluginProcessorEditor::comboBoxChanged(ComboBox* box) {
    if(box == &midiOutputList) {
        setMidiOutput(midiOutputList.getSelectedItemIndex());
    }
}