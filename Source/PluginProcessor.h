#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiOutWorker.h"


// make PluginProcessor::maxMachines of these owned by PluginProcessor, then re-use AudioProcessorValueTreeState's as
// necessary as ExpressionMachines are added/removed.. (by way of setting expressionValueMachine to nullptr here)
class ExpressionMachineValueTreeState
{
public:
    // will need a set state method for use by setStateInformation
    ExpressionMachineValueTreeState(AudioProcessor& processor) :
        parameters(processor, nullptr), expressionValueMachine(nullptr), available(true) {}
    ~ExpressionMachineValueTreeState()
    {
        expressionValueMachine=nullptr;
    }
    bool isAvailable() { return available; }
    void freeMachine() { expressionValueMachine = nullptr; available=true; }
    ExpressionValueMachine* getMachine() { return expressionValueMachine; }
private:
    AudioProcessorValueTreeState parameters;
    ScopedPointer<ExpressionValueMachine> expressionValueMachine;
    bool available;
};

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
    
    int setMidiOutput(int index);
    int setMidiOutput(const String& name);
    const String getMidiOutputName();
    
    const float getExpressionValue() override;
    const int getMidiOutputIndex();
    const bool isActive() override;
    
    void addMachine(const String& typeName);
    

private:
    //==============================================================================
    OwnedArray<ExpressionValueMachine> expressionValueMachines;
    OwnedArray<AudioProcessorValueTreeState> subParameters;
    ScopedPointer<MidiOutWorker> midiOutWorker;
    ScopedPointer<MidiOutputList> midiOutputList;
    AudioProcessorValueTreeState parameters;
    
    float previousThru;
    float currentExpressionValue;
    
    int midiOutputIndex;
    void setMidiOutputName(const String& name);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
