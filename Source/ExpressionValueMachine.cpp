/*
  ==============================================================================

    ExpressionValueMachine.cpp
    Created: 18 Oct 2016 5:56:51pm
    Author:  Oli

  ==============================================================================
*/

#include "ExpressionValueMachine.h"
//#include "Audio2MidiComponent.h"

LoudnessDecayValueMachine::LoudnessDecayValueMachine(AudioProcessorValueTreeState& p, const String& id) : ExpressionValueMachine(p, "LoudnessDecay", id), currentExpressionValue(0)
{
    DBG("LoudnessDecayValueMachine::LoudnessDecayValueMachine");

    createAndAddLocalParameter("decay", "Decay", String(),
        NormalisableRange<float>(0.0f, 0.99f), 0.75f,
        nullptr, nullptr);
     
}

LoudnessDecayValueMachine::~LoudnessDecayValueMachine()
{
}


void LoudnessDecayValueMachine::pushSample(const float& sample)
{
    static const float * const decayParam = getRawLocalParameterValue("decay");
    currentExpressionValue = sample + *decayParam * currentExpressionValue;
}

const float LoudnessDecayValueMachine::getExpressionValue()
{
    return currentExpressionValue;
}

const bool LoudnessDecayValueMachine::isActive()
{
    return true; // not sure why we need this function at the moment
}


Audio2MidiComponent* LoudnessDecayValueMachine::getNewComponent(SubProcessor& parent)
{
    return new LoudnessDecayComponent(parent);
}


ExpressionValueMachine::ExpressionValueMachine(AudioProcessorValueTreeState& p, const String& tn, const String& id) : typeName(tn), uid(id), parameters(p)
{
    jassert(uid != "-1");
    //initializeParametersState();
}



