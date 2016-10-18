/*
  ==============================================================================

    ExpressionValueMachine.cpp
    Created: 18 Oct 2016 5:56:51pm
    Author:  Oli

  ==============================================================================
*/

#include "ExpressionValueMachine.h"


LoudnessDecayValueMachine::LoudnessDecayValueMachine() : decayParam(0.5), currentExpressionValue(0)
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