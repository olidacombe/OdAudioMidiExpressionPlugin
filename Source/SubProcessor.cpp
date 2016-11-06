/*
  ==============================================================================

    SubProcessor.cpp
    Created: 21 Oct 2016 10:39:29pm
    Author:  Oli

  ==============================================================================
*/

#include "SubProcessor.h"

SubProcessor::SubProcessor(AudioProcessorValueTreeState& vts, MidiOutputList* mol, ExpressionValueMachine* mch)
: parameters(vts), machine(mch)
{
    midiOutWorker = new MidiOutWorker(machine);
    midiOutWorker->addChangeListener(this);

    initializeParameters();
    
    component = machine->getNewComponent(*this);
}

SubProcessor::~SubProcessor()
{
    component = nullptr;
    midiOutWorker = nullptr;
    machine = nullptr;
}

void SubProcessor::initializeParameters()
{
    subProcessorParameters = ValueTree(Identifier("SubProcessor"));
    subProcessorParameters.setProperty(Identifier ("type"), machine->getTypeName(), nullptr);

    subProcessorParameters.setProperty(Identifier("uid"), machine->getUid(), nullptr);
    ValueTree midiParameters (Identifier("MidiParameters"));
    
    ValueTree midiOutputParameter (Identifier("Output"));
    midiOutputParameter.setProperty("name", "", nullptr);
    midiParameters.addChild(midiOutputParameter, -1, nullptr);
    
    ValueTree midiChannelParameter(Identifier("Channel"));
    midiChannelParameter.setProperty("number", "1", nullptr);
    midiParameters.addChild(midiChannelParameter, -1, nullptr);
    
    subProcessorParameters.addChild(midiParameters, -1, nullptr);
    
    DBG(String("SubProcessor::initializeParameters() ") + (subProcessorParameters.getParent().isValid() ? "got parent" : "orphan" ));
}


void SubProcessor::setMidiOutputName(const String& name)
{
    DBG(String("SubProcessor::setMidiOutputName ") + name);
    // changes to this didn't seem to reveal themselves in parameters.state - so I clearly misunderstood how ValueTree children work
    //subProcessorParameters.getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).setProperty("name", name, nullptr);
    parameters.state.getChildWithName("SubProcessor").getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).setProperty("name", name, nullptr);
    DBG(subProcessorParameters.getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).getProperty("name").toString());
}

void SubProcessor::changeListenerCallback(ChangeBroadcaster* cb)
{
    if(cb==midiOutWorker && midiOutWorker != nullptr)
    {
        setMidiOutputName(midiOutWorker->getMidiOutputName());
    }
}