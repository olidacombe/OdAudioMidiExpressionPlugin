/*
  ==============================================================================

    ExpressionValueMachine.h
    Created: 18 Oct 2016 5:56:51pm
    Author:  Oli

  ==============================================================================
*/

#ifndef EXPRESSIONVALUEMACHINE_H_INCLUDED
#define EXPRESSIONVALUEMACHINE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "MidiOutWorker.h"

class ExpressionValueSource
{
public:
    virtual ~ExpressionValueSource() {}
    virtual const float getExpressionValue()=0;
    virtual const bool isActive()=0;
};


//class MidiOutWorker;

class ExpressionValueMachine : public ExpressionValueSource
{
public:
    ExpressionValueMachine(AudioProcessorValueTreeState& p, const String& typeName);
    virtual ~ExpressionValueMachine() {}
    //virtual void updateParameters() {}
    virtual void pushSample(const float& sample)=0;
    const String getTypeName() { return typeName; }
    
    
protected:
    const String typeName;
    AudioProcessorParameter* setOrCreateAndAddParameter (String parameterID, String parameterName,
        String labelText, NormalisableRange< float > valueRange, float defaultValue,
        std::function< String(float)> valueToTextFunction,
        std::function< float(const String &)> textToValueFunction);
    
    virtual const float getDefaultValue(const String& parameterID) { return 0.0f; }
    AudioProcessorValueTreeState& parameters;
    void initializeParametersState();
    
private:
    //ScopedPointer<MidiOutWorker> midiOutWorker;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpressionValueMachine)
};


class LoudnessDecayValueMachine : public ExpressionValueMachine
{
public:
    LoudnessDecayValueMachine(AudioProcessorValueTreeState& p);
    ~LoudnessDecayValueMachine();
    //void updateParameters() override;
    void pushSample(const float& sample) override;
    const float getExpressionValue() override;
    const bool isActive() override;
private:
    //float decayParam;
    float currentExpressionValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoudnessDecayValueMachine)
};


#endif  // EXPRESSIONVALUEMACHINE_H_INCLUDED
