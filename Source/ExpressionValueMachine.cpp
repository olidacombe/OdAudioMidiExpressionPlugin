/*
  ==============================================================================

    ExpressionValueMachine.cpp
    Created: 18 Oct 2016 5:56:51pm
    Author:  Oli

  ==============================================================================
*/

#include "ExpressionValueMachine.h"

LoudnessDecayValueMachine::LoudnessDecayValueMachine(AudioProcessorValueTreeState *p) : ExpressionValueMachine(p), decayParam(0.5), currentExpressionValue(0)
{

}

LoudnessDecayValueMachine::~LoudnessDecayValueMachine()
{
}

void LoudnessDecayValueMachine::pushSample(const float& sample)
{
    currentExpressionValue = sample + decayParam * currentExpressionValue;
}

const float LoudnessDecayValueMachine::getExpressionValue()
{
    return currentExpressionValue;
}

const bool LoudnessDecayValueMachine::isActive()
{
    return true; // not sure why we need this function at the moment
}


AudioProcessorParameter* ExpressionValueMachine::setOrCreateAndAddParameter (String parameterID, String parameterName,
        String labelText, NormalisableRange< float > valueRange, float defaultValue,
        std::function< String(float)> valueToTextFunction,
        std::function< float(const String &)> textToValueFunction)
{
    AudioProcessorParameter* p = parameters->getParameter(parameterID);
    if(p==nullptr) {
        return parameters->createAndAddParameter(parameterID, parameterName, labelText, valueRange,
            defaultValue, valueToTextFunction, textToValueFunction);
    } else {
        p->setValue(getDefaultValue(parameterID));
        return p;
    }
}
