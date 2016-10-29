#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiOutWorker.h"
#include "ExpressionValueMachine.h"
#include "SubProcessor.h"


//==============================================================================
/**
*/
class PluginProcessor  : public AudioProcessor
{
public:

    enum { maxMachines = 4 };

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
    
    SubProcessor& getSubProcessor(int index);
    


private:
    //==============================================================================

    void initializeState();
    
    OwnedArray<SubProcessor, CriticalSection> subProcessors;
    
    SharedResourcePointer<MidiOutputList> midiOutputList;
    
    // [ to go ]
    ScopedPointer<MidiOutWorker> midiOutWorker;
    float previousThru;
    float currentExpressionValue;
    int midiOutputIndex;
    void setMidiOutputName(const String& name);
    // [ / to go ]
    
    AudioProcessorValueTreeState parameters;
    

    
    template <typename T> SubProcessor* addSubProcessor();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
