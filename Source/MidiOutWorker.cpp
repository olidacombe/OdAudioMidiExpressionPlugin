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
    //Thread("Midi Output Worker"),
    processor(p),
    lastOutputCCValue(0), currentOutputCCValue(0)
{

}

MidiOutWorker::~MidiOutWorker()
{
    //stopThread(2000);
    
}

void MidiOutWorker::timerCallback()
{    
    float expression = processor->getExpressionValue();    
    currentOutputCCValue = 127.0 * expression;
    if(currentOutputCCValue != lastOutputCCValue) {
        sendMessage();
        lastOutputCCValue = currentOutputCCValue;
    }
}

void MidiOutWorker::sendMessage() {
    MidiMessage message = MidiMessage::controllerEvent(1, 20, currentOutputCCValue);
    DBG(String("sending message ") + String(currentOutputCCValue));
    midiOutput->sendMessageNow(message);
}

int MidiOutWorker::setMidiOutput(int index) {
    
    stopTimer();
    midiOutput = nullptr;

    if (MidiOutput::getDevices() [index].isNotEmpty())
    {
        midiOutput = MidiOutput::openDevice (index);
        jassert (midiOutput);
        /*
        MidiMessage message = MidiMessage::controllerEvent(1, 20, 99);
        DBG("sending message");
        midiOutput->sendMessageNow(message);
        */
        
        startTimer(300);
        
        return index+1; // appropriate for ComboBox
    } 
    return 0;
}