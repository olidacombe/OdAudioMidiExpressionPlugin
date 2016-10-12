#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OdAudioMidiExpressionPluginAudioProcessor::OdAudioMidiExpressionPluginAudioProcessor()
:   currentExpressionValue(0.0)
{
    midiOutWorker = new MidiOutWorker(this);
}

OdAudioMidiExpressionPluginAudioProcessor::~OdAudioMidiExpressionPluginAudioProcessor()
{
    midiOutWorker = nullptr;
}

//==============================================================================
const String OdAudioMidiExpressionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OdAudioMidiExpressionPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OdAudioMidiExpressionPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double OdAudioMidiExpressionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OdAudioMidiExpressionPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OdAudioMidiExpressionPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OdAudioMidiExpressionPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String OdAudioMidiExpressionPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void OdAudioMidiExpressionPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OdAudioMidiExpressionPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OdAudioMidiExpressionPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdAudioMidiExpressionPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
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

void OdAudioMidiExpressionPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
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

int OdAudioMidiExpressionPluginAudioProcessor::setMidiOutput(int index) {
    return midiOutWorker->setMidiOutput(index);
}

//==============================================================================
bool OdAudioMidiExpressionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OdAudioMidiExpressionPluginAudioProcessor::createEditor()
{
    return new OdAudioMidiExpressionPluginAudioProcessorEditor (*this);
}

//==============================================================================
void OdAudioMidiExpressionPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OdAudioMidiExpressionPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float OdAudioMidiExpressionPluginAudioProcessor::getExpressionValue() {
    return currentExpressionValue;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OdAudioMidiExpressionPluginAudioProcessor();
}
