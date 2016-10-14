#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginProcessor::PluginProcessor()
:   currentExpressionValue(0.0), parameters(*this, nullptr)
{
    parameters.createAndAddParameter ("thru", "Thru", String(),
                                          NormalisableRange<float> (0.0f, 1.0f, 1.0f), 0.0f,
                                          thruToText,    // value to text function
                                          textToThru);   // text to value function
    
    midiOutWorker = new MidiOutWorker(this);
}

PluginProcessor::~PluginProcessor()
{
    midiOutWorker = nullptr;
}


String PluginProcessor::thruToText (float value)
{
    return value < 0.5 ? "Normal" : "Inverted";
}
    
float PluginProcessor::textToThru (const String& text)
{
    if (text == "Normal")    return 0.0f;
    if (text == "Inverted")  return 1.0f;
    return 0.0f;
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    float incomingLoudness;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        incomingLoudness += buffer.getRMSLevel(channel, 0, numSamples);
    }
    
    // go atomic...?
    currentExpressionValue = 0.9*(currentExpressionValue + incomingLoudness);
}

int PluginProcessor::setMidiOutput(int index) {
    return midiOutWorker->setMidiOutput(index);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginProcessorEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float PluginProcessor::getExpressionValue() {
    return currentExpressionValue;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
