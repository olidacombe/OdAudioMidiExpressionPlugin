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


void SubProcessor::setSubProcessorParameters(const ValueTree& spp)
{
    subProcessorParameters = spp;
    // problem, this calls parameters.state.... when spp hasn't been re-added yet
    
    // so NOTE 1 should probably add subProcessorParameters to parameters here
    // then set a funciton to rip through subProcessors and add their subParameters
    // to parameters if this wasn't called
    
    //setMidiOutputName(getParametersMidiOutputName());
}


void SubProcessor::setMidiOutputName(const String& name)
{
    DBG(String("SubProcessor::setMidiOutputName ") + name);
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