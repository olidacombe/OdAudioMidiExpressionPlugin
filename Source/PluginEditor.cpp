#include "PluginEditor.h"


//==============================================================================
OdAudioMidiExpressionPluginAudioProcessorEditor::OdAudioMidiExpressionPluginAudioProcessorEditor (OdAudioMidiExpressionPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(midiOutputList);
    midiOutputList.setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
    const StringArray midiOutputs(MidiOutput::getDevices());
    midiOutputList.addItemList(midiOutputs, 1);
    midiOutputList.addListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

OdAudioMidiExpressionPluginAudioProcessorEditor::~OdAudioMidiExpressionPluginAudioProcessorEditor()
{
    //midiOutput = nullptr;
}

//==============================================================================
void OdAudioMidiExpressionPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void OdAudioMidiExpressionPluginAudioProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    midiOutputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
}

void OdAudioMidiExpressionPluginAudioProcessorEditor::setMidiOutput(int index) {
    //midiOutput = MidiOutput::openDevice(index);
    //if(midiOutput != nullptr) {
    //    midiOutputList.setSelectedId(index + 1, dontSendNotification);
    //    processor.setMidiOutput(index);
    //}
    midiOutputList.setSelectedId(processor.setMidiOutput(index));
}

void OdAudioMidiExpressionPluginAudioProcessorEditor::comboBoxChanged(ComboBox* box) {
    if(box == &midiOutputList) {
        setMidiOutput(midiOutputList.getSelectedItemIndex());
    }
}