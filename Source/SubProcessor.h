/*
  ==============================================================================

    SubProcessor.h
    Created: 21 Oct 2016 10:39:29pm
    Author:  Oli

  ==============================================================================
*/

#ifndef SUBPROCESSOR_H_INCLUDED
#define SUBPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ExpressionValueMachine.h"

/*
class MidiOutputList;
class MidiOutWorker;
class ExpressionValueMachine;
*/

class SubProcessor
{
public:

    SubProcessor(AudioProcessorValueTreeState&, MidiOutputList&, ExpressionValueMachine*);
    ~SubProcessor();
    
private:

    ScopedPointer<ExpressionValueMachine> machine;
    ScopedPointer<MidiOutWorker> midiOutWorker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubProcessor)
};

#endif  // SUBPROCESSOR_H_INCLUDED
