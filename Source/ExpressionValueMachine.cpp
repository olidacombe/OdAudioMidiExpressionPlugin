/*
  ==============================================================================

    ExpressionValueMachine.cpp
    Created: 18 Oct 2016 5:56:51pm
    Author:  Oli

  ==============================================================================
*/

#include "ExpressionValueMachine.h"

LoudnessDecayValueMachine::LoudnessDecayValueMachine(AudioProcessorValueTreeState& p, const String& id) : ExpressionValueMachine(p, "LoudnessDecay", id), /*decayParam(0.5),*/ currentExpressionValue(0)
{
    DBG("LoudnessDecayValueMachine::LoudnessDecayValueMachine");
    /*
    setOrCreateAndAddParameter(uid+".decay", "Decay", String(),
        NormalisableRange<float>(0.0f, 0.99f), 0.75f,
        nullptr, nullptr);
    */
    createAndAddLocalParameter("decay", "Decay", String(),
        NormalisableRange<float>(0.0f, 0.99f), 0.75f,
        nullptr, nullptr);
     
}

LoudnessDecayValueMachine::~LoudnessDecayValueMachine()
{
}


void LoudnessDecayValueMachine::pushSample(const float& sample)
{
    const float *decayParam = parameters.getRawParameterValue(uid+".decay");
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
    //initializeParametersState();
}

/*
    this is kind of looking ahead to a world where we let the user dynamically make new "tabs" of different machine
    types, re-order them etc.  Due to AudioProcessorValueTreeState needing to have the same lifespan as the processor,
    we'd better be re-using those objects - and you don't know what type yours used to have so...
    Also, at save time, it'd be best to prune off any parameter values which don't apply to their assigned processor
    from the ValueTree before going to XML->disk.
*/ /*
AudioProcessorParameter* ExpressionValueMachine::setOrCreateAndAddParameter (String parameterID, String parameterName,
        String labelText, NormalisableRange< float > valueRange, float defaultValue,
        std::function< String(float)> valueToTextFunction,
        std::function< float(const String &)> textToValueFunction)
{
    DBG("setOrCreateAndAddParameter");
    AudioProcessorParameter* p = parameters.getParameter(parameterID);
    if(p==nullptr) {
        DBG("setOfCreateAndAddParameter : not found -> create");
        return parameters.createAndAddParameter(parameterID, parameterName, labelText, valueRange,
            defaultValue, valueToTextFunction, textToValueFunction);
    } else {
        // this isn't taking care of parameters with the same name from different machines having different
        // defaults, ranges, text functions etc :\
        DBG("setOrCreateAndAddParameter : found -> setValue");
        p->setValue(getDefaultValue(parameterID));
        return p;
    }
}
*/



