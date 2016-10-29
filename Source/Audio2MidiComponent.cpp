/*
  ==============================================================================

    Audio2MidiComponent.cpp
    Created: 18 Oct 2016 5:48:31pm
    Author:  Oli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Audio2MidiComponent.h"
#include "LevelMeter.h"
//#include "SubProcessor.h"

MidiOutputComboBox::MidiOutputComboBox(MidiOutWorker* mow) : midiOutputWorker(mow)
{
    midiOutputWorker->addChangeListener(this);
    refreshList();
}

void MidiOutputComboBox::changeListenerCallback(ChangeBroadcaster* cb)
{
    DBG("MidiOutputComboBox::changeListenerCallback");
    if(cb == midiOutputWorker)
    {
        refreshList();
    }
}

void MidiOutputComboBox::refreshList() {
    const StringArray midiOutputs(MidiOutput::getDevices());
    //const int selectedOutput = processor.getMidiOutputIndex();

    // quick and dirty way to address menu update while open
    if(isPopupActive()) {
        hidePopup();
    }
    clear(dontSendNotification);
    addItemList(midiOutputs, 1);
    //midiOutputList.setSelectedId(selectedOutput + 1);
}


//==============================================================================
Audio2MidiComponent::Audio2MidiComponent(SubProcessor& sp)
: subProcessor(sp), parameters(sp.getParameters())
{
    midiOutputComboBox = new MidiOutputComboBox(subProcessor.getMidiOutWorker());
    midiOutputComboBox->setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
    addAndMakeVisible(midiOutputComboBox);
    midiOutputComboBox->addListener(subProcessor.getMidiOutWorker());
    
    levelMeter = new LevelMeter();
    addAndMakeVisible(levelMeter);
}


Audio2MidiComponent::~Audio2MidiComponent()
{
    midiOutputComboBox = nullptr;
    levelMeter = nullptr;
}



void Audio2MidiComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("Audio2MidiComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

String Audio2MidiComponent::localizedParameterID(const String& parameterID) { return subProcessor.getMachine()->localizedParameterID(parameterID); }

LoudnessDecayComponent::LoudnessDecayComponent(SubProcessor& sp)
: Audio2MidiComponent(sp)
{
    decayLabel.setText("Decay", dontSendNotification);
    addAndMakeVisible(decayLabel);
    decaySlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(decaySlider);
    decayAttachment = new SliderAttachment (parameters, localizedParameterID("decay"), decaySlider);
}

LoudnessDecayComponent::~LoudnessDecayComponent()
{
    
}   

void LoudnessDecayComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background

    //g.setColour (Colours::orange);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("LoudnessDecayComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void LoudnessDecayComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    Rectangle<int> area = getLocalBounds();
    levelMeter->setBounds(area.removeFromRight(24).reduced(4));
    Rectangle<int> bottomBar = area.removeFromBottom(30);
    midiOutputComboBox->setBounds(bottomBar.removeFromLeft(100).reduced(4));

    Rectangle<int> decayBox = area.removeFromTop(50).reduced(10);
    decaySlider.setBounds(decayBox.removeFromLeft(100).reduced(5));
    decayLabel.setBounds(decayBox.removeFromLeft(50));
}
