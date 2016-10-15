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

class ExpressionValueSource
{
public:
    virtual ~ExpressionValueSource() {};
    virtual float getExpressionValue()=0;
};

/*

This will be responsible for sending out midi messages.  I may "unthread" this,
and substitue an object which calculates (or just reads) the current "expression value"
to be sent - making that a worker thread.

*/

class MidiOutputList : public Timer, public ChangeBroadcaster
{
public:
    MidiOutputList();
    ~MidiOutputList();
    void timerCallback() override;
private:
    StringArray currentOutputList;

    bool hasOutputListChanged();
};


class PluginProcessor;

class MidiOutWorker : public Timer
{
public:
    MidiOutWorker(ExpressionValueSource* e);
    ~MidiOutWorker();
    void timerCallback() override;
    int setMidiOutput(int index);
    int setMidiOutput(const String& midiOutName);
    const String& getMidiOutputName();
    void stop();
private:
    void sendMessage();
    int lastOutputCCValue, currentOutputCCValue;
    ExpressionValueSource *evs;
    ScopedPointer<MidiOutput> midiOutput;
};


#endif  // MIDIOUTWORKER_H_INCLUDED
