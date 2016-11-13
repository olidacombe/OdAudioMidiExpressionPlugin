#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
PluginProcessor::PluginProcessor()
:   parameters(*this, nullptr), currentExpressionValue(0.0), midiOutputIndex(0)
{

    parameters.createAndAddParameter ("thru", "Thru", String(),
        NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
        [](float value)
        {
          // value to text function
          return value < 0.5 ? "Mute" : "Thru";
        },
        [](const String& text)
        {
          // text to value function
          if (text == "Mute")    return 0.0f;
          if (text == "Thru")  return 1.0f;
          return 0.0f;
        }
    );
    
    parameters.createAndAddParameter("active", "Active", String(),
        NormalisableRange<float> (0.0f, 1.0f, 1.0f), 1.0f,
        [](float value)
        {
            return value > 0.5 ? "Active" : "Inactive";
        },
        [](const String& text)
        {
            if(text == "Active") return 1.0f;
            if(text == "Thru") return 0.0f;
            return 0.0f;
        }
    );

    
    // all parameters have to be created before giving AudioProcessorValueTreeState
    // a ValueTree!
    addSubProcessor<LoudnessDecayValueMachine>();
    
    initializeState();

    
}

void PluginProcessor::initializeState()
{
    // must stay!
    parameters.state = ValueTree (Identifier ("OdAudioMidiExpressionPlugin"));
    
    for(SubProcessor* subProc : subProcessors)
    {
        parameters.state.addChild(subProc->state(), -1, nullptr);
    }
}


PluginProcessor::~PluginProcessor()
{
    //midiOutputList = nullptr;
    midiOutWorker = nullptr;
}


//==============================================================================
const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const String PluginProcessor::getProgramName (int index)
{
    return String();
}

void PluginProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // are we sure this runs before an editor is created?
    //setMidiOutput(parameters.state.getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).getProperty("name"));

    previousThru = *parameters.getRawParameterValue ("thru");
}


void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    const float currentThru = *parameters.getRawParameterValue ("thru");

    // I dislike this being here a bit
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    float incomingLoudness = 0;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        incomingLoudness += buffer.getRMSLevel(channel, 0, numSamples);
    }
    
    /*
    for(ExpressionValueMachine* evm : expressionValueMachines) {
        // do some shit
        evm->pushSample(incomingLoudness);
    }
    */
    
    for(SubProcessor* sp : subProcessors)
    {
        sp->getMachine()->pushSample(incomingLoudness);
    }
    
    
    
    // go atomic...?
    //currentExpressionValue = decayParam * (currentExpressionValue + incomingLoudness);
    
    if (currentThru == previousThru)
    {
        //buffer.applyGain (currentGain);
        if(currentThru == 0.0f) buffer.clear();
    }
    else
    {
        buffer.applyGainRamp (0, numSamples, previousThru, currentThru);
        previousThru = currentThru;
    }
}


//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginProcessorEditor (*this, parameters);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{   
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
    
    DBG("getStateInformation:");
    DBG(parameters.state.toXmlString());
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // return; // schema changes can appear not to happen when this function recalls
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
        
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {  
            // here - our referenced child is replaced!
            // so following setStateInformation, methods inside SubParameter which modify
            // subParameterState aren't reflecting in the main AudioProcessorValueTreeState
            // although I've confirmed when originally made that stuff worked fine
            // so this needs the references repairing after happening.
            // Maybe just re-assign SubProcessor.subParameter in this function.  Yes
            parameters.state = ValueTree::fromXml (*xmlState);
            
            /*
            here iterate through SubProcessor children spc, look for a SP with same uid, and
            call:
                setSubProcessorParameters(spc);
            
            any SubProcessors in subProcessors left out must then have their subProcessorParameters
            added to parameters:
                parameters.state.addChild(subProc->state(), -1, nullptr);
            */
            
            ValueTree sp;
            while((sp = parameters.state.getChildWithName("SubProcessor")).isValid())
            {
                // whatever happens we delete this here
                parameters.state.removeChild(sp, nullptr);
                
                DBG(String("setStateInformation child scan: ") + sp.getProperty("uid", -1).toString());
                const String uid = sp.getProperty("uid", -1).toString();
                
                // uid should never be -1 ... asserted in ExpressionValueMachine constructor
                SubProcessor* foundSubProcessor = getSubProcessorByUid(uid);
                
                if(foundSubProcessor!=nullptr)
                {
                    DBG("setStateInformation - setSubProcessorParameters");
                    foundSubProcessor->setSubProcessorParameters(sp);
                }

            }
            
            for(SubProcessor* sp : subProcessors)
            {
                parameters.state.addChild(sp->state(), -1, nullptr);
            }
        }
    }
    
    DBG("setStateInformation:");
    DBG(parameters.state.toXmlString());
            
    
}


template <typename T>
SubProcessor* PluginProcessor::addSubProcessor()
{
    static int nonce = 0;

    return subProcessors.add(new SubProcessor(parameters, midiOutputList, new T(parameters, String(nonce++))));
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

SubProcessor* PluginProcessor::getSubProcessorByUid(const String& uid)
{
    for(SubProcessor* sp : subProcessors)
    {
        if(sp->getMachine()->getUid() == uid)
        {
            return sp;
        }
    }
    return nullptr;
}

// this won't be done in such a way
SubProcessor& PluginProcessor::getSubProcessor(int index)
{
    return *subProcessors[index];
}
