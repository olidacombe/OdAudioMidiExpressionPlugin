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
    updateAvailableMidiOutputList();
    midiOutputList.addListener(this);
    
    processor.addChangeListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

PluginProcessorEditor::~PluginProcessorEditor()
{
    processor.removeChangeListener(this);
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
    midiOutputList.setSelectedId(processor.setMidiOutput(index) + 1);
}

void PluginProcessorEditor::comboBoxChanged(ComboBox* box) {
    if(box == &midiOutputList) {
        setMidiOutput(midiOutputList.getSelectedItemIndex());
    }
}

void PluginProcessorEditor::updateAvailableMidiOutputList() {
    const StringArray midiOutputs(MidiOutput::getDevices());
    // quick and dirty way to address menu update while open
    if(midiOutputList.isPopupActive()) {
        midiOutputList.hidePopup();
    }
    midiOutputList.clear(dontSendNotification);
    midiOutputList.addItemList(midiOutputs, 1);
}

void PluginProcessorEditor::changeListenerCallback(ChangeBroadcaster* src) {
    updateAvailableMidiOutputList();
    //midiOutputList.setSelectedId(processor.getMidiOutput() + 1);
}