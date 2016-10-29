#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiOutWorker.h"
#include "ExpressionValueMachine.h"
#include "SubProcessor.h"


//==============================================================================
/**
*/
class PluginProcessor  : public AudioProcessor,
                        public ExpressionValueSource,
                        public ChangeBroadcaster,
                        public ChangeListener
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
    
    void changeListenerCallback(ChangeBroadcaster* src) override;
    
    // slated for removal
    int setMidiOutput(int index);
    int setMidiOutput(const String& name);
    const String getMidiOutputName();
    
    const float getExpressionValue() override;
    const int getMidiOutputIndex();
    const bool isActive() override;
    
    ExpressionValueMachine* addMachine(const String& typeName);
    // end slated for removal
    
    SubProcessor& getSubProcessor(int index);
    


private:
    //==============================================================================

    // no no noo, 3 arrays with dependant indices
    // create a subProcessor class to contain the buddies!
    
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
