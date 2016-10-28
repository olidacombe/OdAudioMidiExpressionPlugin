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

//==============================================================================
Audio2MidiComponent::Audio2MidiComponent(SubProcessor& sp)
: subProcessor(sp), parameters(sp.getParameters())
{
    midiOutputComboBox = new MidiOutputComboBox(&subProcessor.getMidiOutWorker());
    addAndMakeVisible(midiOutputComboBox);
    
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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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
