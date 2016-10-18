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

class ExpressionValueSource
{
public:
    virtual ~ExpressionValueSource() {};
    virtual const float getExpressionValue()=0;
    virtual const bool isActive()=0;
};

class ExpressionValueMachine : public ExpressionValueSource
{
public:
    ExpressionValueMachine();
    virtual ~ExpressionValueMachine()=0;
    virtual void pushSample(const float& sample)=0;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpressionValueMachine)
};


class LoudnessDecayValueMachine : public ExpressionValueMachine
{
public:
    LoudnessDecayValueMachine();
    ~LoudnessDecayValueMachine();
    void pushSample(const float& sample) override;
    const float getExpressionValue() override;
    const bool isActive() override;
private:
    float decayParam;
    float currentExpressionValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoudnessDecayValueMachine)
};


#endif  // EXPRESSIONVALUEMACHINE_H_INCLUDED
