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
//#include "Audio2MidiComponent.h"

class Audio2MidiComponent;

class SubProcessor
{
public:

    SubProcessor(AudioProcessorValueTreeState&, MidiOutputList*, ExpressionValueMachine*);
    ~SubProcessor();
    
    AudioProcessorValueTreeState& getParameters() { return parameters; }
    void setParameters(const ValueTree& source);
    MidiOutWorker& getMidiOutWorker() { return *midiOutWorker; }
    ValueTree& state() { return parameters.state; }
    
    Audio2MidiComponent* getComponent() { return component; }
    
private:
    // maybe instances should own their Audio2MidiComponent ?
    AudioProcessorValueTreeState& parameters;
    ScopedPointer<Audio2MidiComponent> component;
    ScopedPointer<ExpressionValueMachine> machine;
    ScopedPointer<MidiOutWorker> midiOutWorker;

    void initializeParameters();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubProcessor)
};

#endif  // SUBPROCESSOR_H_INCLUDED
