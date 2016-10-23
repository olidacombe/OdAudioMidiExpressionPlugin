/*
  ==============================================================================

    SubProcessor.cpp
    Created: 21 Oct 2016 10:39:29pm
    Author:  Oli

  ==============================================================================
*/

#include "SubProcessor.h"

SubProcessor::SubProcessor(AudioProcessorValueTreeState* vts, MidiOutputList* mol, ExpressionValueMachine* mch)
: machine(mch)
{
    midiOutWorker = new MidiOutWorker(machine, mol);
}

SubProcessor::~SubProcessor()
{
    midiOutWorker = nullptr;
    machine = nullptr;
}
