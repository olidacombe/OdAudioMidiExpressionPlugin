/*
  ==============================================================================

    MidiOutWorker.h
    Created: 12 Oct 2016 2:30:10pm
    Author:  Oli

  ==============================================================================
*/

#ifndef MIDIOUTWORKER_H_INCLUDED
#define MIDIOUTWORKER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

/*

This will be responsible for sending out midi messages.  I may "unthread" this,
and substitue an object which calculates (or just reads) the current "expression value"
to be sent - making that a worker thread.

*/

class OdAudioMidiExpressionPluginAudioProcessor;

class MidiOutWorker : public Timer
{
public:
    MidiOutWorker(OdAudioMidiExpressionPluginAudioProcessor* p);
    ~MidiOutWorker();
    void timerCallback() override;
    int setMidiOutput(int index);
private:
    void sendMessage();
    int lastOutputCCValue, currentOutputCCValue;
    OdAudioMidiExpressionPluginAudioProcessor *processor;
    ScopedPointer<MidiOutput> midiOutput;
};


#endif  // MIDIOUTWORKER_H_INCLUDED
