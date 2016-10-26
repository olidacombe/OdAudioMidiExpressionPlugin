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
    parameters.state = ValueTree(Identifier("SubProcessor"));
    parameters.state.setProperty(Identifier ("type"), machine->getTypeName(), nullptr);
    ValueTree midiParameters (Identifier("MidiParameters"));
    
    ValueTree midiOutputParameter (Identifier("Output"));
    midiOutputParameter.setProperty("name", "", nullptr);
    midiParameters.addChild(midiOutputParameter, -1, nullptr);
    
    ValueTree midiChannelParameter(Identifier("Channel"));
    midiChannelParameter.setProperty("number", "1", nullptr);
    midiParameters.addChild(midiChannelParameter, -1, nullptr);
    
    parameters.state.addChild(midiParameters, -1, nullptr);
    
/*  // an experiment based on worry, based on the appearance that
    // all AudoProcessorValueTreeState saves are producing the same
    // list of parameters... differently populated :S
    float *changeMe = parameters.getRawParameterValue("active");
    if(changeMe != nullptr)
        *changeMe=0.5;
*/
}


void SubProcessor::setParameters(const ValueTree& source)
{
    ValueTree machineParams = source.getChildWithName(Identifier(machine->getTypeName()));
    if(machineParams.isValid()) {
        parameters.state.copyPropertiesFrom(source, nullptr);
    }
}
