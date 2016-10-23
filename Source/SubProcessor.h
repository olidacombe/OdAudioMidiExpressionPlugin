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


class SubProcessor
{
public:

    SubProcessor(AudioProcessorValueTreeState&, MidiOutputList*, ExpressionValueMachine*);
    ~SubProcessor();
    
    void setParameters(const ValueTree& source);
    
private:
    AudioProcessorValueTreeState& parameters;
    ScopedPointer<ExpressionValueMachine> machine;
    ScopedPointer<MidiOutWorker> midiOutWorker;

    void initializeParameters();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubProcessor)
};

#endif  // SUBPROCESSOR_H_INCLUDED
