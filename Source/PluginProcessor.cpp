#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginProcessor::PluginProcessor()
:   currentExpressionValue(0.0), parameters(*this, nullptr), midiOutputIndex(0)
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
    });
    
    parameters.state = ValueTree (Identifier ("OdAudioMidiExpressionPlugin"));
    ValueTree midiParameters (Identifier("MidiParameters"));
    ValueTree midiOutputParameter (Identifier("Output"));
    midiOutputParameter.setProperty("name", "", nullptr);
    midiParameters.addChild(midiOutputParameter, -1, nullptr);
    parameters.state.addChild(midiParameters, -1, nullptr);
                                          
    midiOutWorker = new MidiOutWorker(this);
    midiOutputList = new MidiOutputList();
    midiOutputList->addChangeListener(this);

}

PluginProcessor::~PluginProcessor()
{
    midiOutputList = nullptr;
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
        
    setMidiOutput(getMidiOutputName());
    sendChangeMessage();
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
    
    const float currentThru = *parameters.getRawParameterValue ("thru");

    // I dislike this being here a bit
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    float incomingLoudness;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        incomingLoudness += buffer.getRMSLevel(channel, 0, numSamples);
    }
    
    // go atomic...?
    currentExpressionValue = 0.9*(currentExpressionValue + incomingLoudness);
    
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

int PluginProcessor::setMidiOutput(int index) {
    const int outIndex = midiOutWorker->setMidiOutput(index);
    String midiOutName("");
    if(outIndex != -1) {
        midiOutName = midiOutWorker->getMidiOutputName();
    }
    setMidiOutputName(midiOutName);
    midiOutputIndex = outIndex;
    return outIndex;
}

int PluginProcessor::setMidiOutput(const String& name) {
    const int outIndex = midiOutWorker->setMidiOutput(name);
    midiOutputIndex = outIndex;
    return outIndex;
}



const String PluginProcessor::getMidiOutputName() {
    return(parameters.state.getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).getProperty("name"));
}

void PluginProcessor::setMidiOutputName(const String& name) {
    parameters.state.getChildWithName(Identifier("MidiParameters")).getChildWithName(Identifier("Output")).setProperty("name", name, nullptr);
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
    #if JUCE_DEBUG
    const File xmlDebugDump ("/Users/oli/lol.xml");
    xml->writeToFile(xmlDebugDump, "");
    #endif
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // return; // schema changes can appear not to happen when this function recalls
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
        
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

float PluginProcessor::getExpressionValue() {
    return currentExpressionValue;
}

const int PluginProcessor::getMidiOutputIndex() {
    return midiOutputIndex;
}

void PluginProcessor::changeListenerCallback(ChangeBroadcaster* src) {
    if(src == midiOutputList) {
        midiOutWorker->stop();
        
        setMidiOutput(getMidiOutputName());
        
        sendChangeMessage();
    }
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
