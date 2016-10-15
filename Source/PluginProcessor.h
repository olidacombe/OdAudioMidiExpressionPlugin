#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiOutWorker.h"


//==============================================================================
/**
*/
class PluginProcessor  : public AudioProcessor,
                        public ExpressionValueSource,
                        public ChangeBroadcaster,
                        public ChangeListener
{
public:

    //==============================================================================
    PluginProcessor();
    ~PluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void changeListenerCallback(ChangeBroadcaster* src) override;
    
    int setMidiOutput(int index);
    const String getMidiOutputName();
    
    
    float getExpressionValue() override;
    

private:
    //==============================================================================
    ScopedPointer<MidiOutWorker> midiOutWorker;
    ScopedPointer<MidiOutputList> midiOutputList;
    AudioProcessorValueTreeState parameters;
    
    float previousThru;
    float currentExpressionValue;
    
    void setMidiOutputName(const String& name);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
