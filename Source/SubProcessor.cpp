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
    midiOutWorker = new MidiOutWorker(machine, mol);
    
    // kind of redundant when there's already a stored state - meh
    initializeParameters();
}

SubProcessor::~SubProcessor()
{
    midiOutWorker = nullptr;
    machine = nullptr;
}

void SubProcessor::initializeParameters()
{
    parameters.state = ValueTree (Identifier (machine->getTypeName()));
    ValueTree midiParameters (Identifier("MidiParameters"));
    
    ValueTree midiOutputParameter (Identifier("Output"));
    midiOutputParameter.setProperty("name", "", nullptr);
    midiParameters.addChild(midiOutputParameter, -1, nullptr);
    
    ValueTree midiChannelParameter(Identifier("Channel"));
    midiChannelParameter.setProperty("number", "1", nullptr);
    midiParameters.addChild(midiChannelParameter, -1, nullptr);
    
    parameters.state.addChild(midiParameters, -1, nullptr);
}

void SubProcessor::setParameters(const ValueTree& source)
{
    ValueTree machineParams = source.getChildWithName(Identifier(machine->getTypeName()));
    if(machineParams.isValid()) {
        parameters.state.copyPropertiesFrom(source, nullptr);
    }
}