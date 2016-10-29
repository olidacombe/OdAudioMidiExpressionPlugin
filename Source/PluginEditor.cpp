#include "PluginEditor.h"


//==============================================================================
PluginProcessorEditor::PluginProcessorEditor (PluginProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    
    thruButton.setButtonText ("Thru");
    addAndMakeVisible (thruButton);
    thruAttachment = new ButtonAttachment (valueTreeState, "thru", thruButton);
    
    activeButton.setButtonText("Active");
    addAndMakeVisible(activeButton);
    activeAttachment = new ButtonAttachment (valueTreeState, "active", activeButton);

    subProcessorComponent = processor.getSubProcessor(0).getComponent();
    addAndMakeVisible(subProcessorComponent);
    
    
    setSize (400, 300);
}

PluginProcessorEditor::~PluginProcessorEditor()
{
}

//==============================================================================
void PluginProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::grey);

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void PluginProcessorEditor::resized()
{
    Rectangle<int> area(getLocalBounds());
    Rectangle<int> topBar = area.removeFromTop(36);
    thruButton.setBounds(topBar.removeFromLeft(70));
    activeButton.setBounds(topBar.removeFromLeft(70));
    
    subProcessorComponent->setBounds(area.reduced(2));
}

