#include "PluginEditor.h"


//==============================================================================
PluginProcessorEditor::PluginProcessorEditor (PluginProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    decayLabel.setText("Decay", dontSendNotification);
    addAndMakeVisible(decayLabel);
    decaySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(decaySlider);
    decayAttachment = new SliderAttachment (valueTreeState, "decay", decaySlider);
    
    thruButton.setButtonText ("Thru");
    addAndMakeVisible (thruButton);
    thruAttachment = new ButtonAttachment (valueTreeState, "thru", thruButton);
    
    activeButton.setButtonText("Active");
    addAndMakeVisible(activeButton);
    activeAttachment = new ButtonAttachment (valueTreeState, "active", activeButton);
    
    
    addAndMakeVisible(midiOutputList);
    midiOutputList.setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
    //updateAvailableMidiOutputList();
    midiOutputList.addListener(this);
    
    processor.addChangeListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    loudnessDecayComponent = new LoudnessDecayComponent(p, 
    
    
    
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
    g.fillAll (Colours::grey);

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void PluginProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    Rectangle<int> topBar = area.removeFromTop(36);
    thruButton.setBounds(topBar.removeFromLeft(70));
    activeButton.setBounds(topBar.removeFromLeft(70));
    //midiOutputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
    midiOutputList.setBounds(topBar.removeFromRight(150).reduced(8));
    Rectangle<int> decayBox = area.removeFromTop(50);
    decaySlider.setBounds(decayBox.removeFromLeft(100));
    decayLabel.setBounds(decayBox.removeFromLeft(50));
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
    const int selectedOutput = processor.getMidiOutputIndex();

    // quick and dirty way to address menu update while open
    if(midiOutputList.isPopupActive()) {
        midiOutputList.hidePopup();
    }
    midiOutputList.clear(dontSendNotification);
    midiOutputList.addItemList(midiOutputs, 1);
    midiOutputList.setSelectedId(selectedOutput + 1);
}

void PluginProcessorEditor::changeListenerCallback(ChangeBroadcaster* src) {
    updateAvailableMidiOutputList();
    //midiOutputList.setSelectedId(processor.getMidiOutput() + 1);
}
