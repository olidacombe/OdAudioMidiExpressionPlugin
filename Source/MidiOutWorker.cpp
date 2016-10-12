/*
  ==============================================================================

    MidiOutWorker.cpp
    Created: 12 Oct 2016 2:30:10pm
    Author:  Oli

  ==============================================================================
*/

#include "MidiOutWorker.h"
#include "PluginProcessor.h"


MidiOutWorker::MidiOutWorker(OdAudioMidiExpressionPluginAudioProcessor* p)
:
    Thread("Midi Output Worker"),
    processor(p)
{

}

MidiOutWorker::~MidiOutWorker()
{
    stopThread(2000);
}

void MidiOutWorker::run()
{
    const MessageManagerLock mml (Thread::getCurrentThread());

    if (! mml.lockWasGained())
        return;
    
    float expression = processor->getExpressionValue();    
    
    // mml falls out of scope and gets deleted
}

int MidiOutWorker::setMidiOutput(int index) {
    
    midiOutput = nullptr;

    if (MidiOutput::getDevices() [index].isNotEmpty())
    {
        midiOutput = MidiOutput::openDevice (index);
        jassert (midiOutput);
        MidiMessage message = MidiMessage::controllerEvent(1, 20, 99);
        DBG("sending message");
        midiOutput->sendMessageNow(message);
        return index+1; // appropriate for ComboBox
    } 
    return 0;
}