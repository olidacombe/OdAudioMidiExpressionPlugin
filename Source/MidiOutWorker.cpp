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
    processor(p)
{

}

MidiOutWorker::~MidiOutWorker()
{
    //stopThread(2000);
    
}

void MidiOutWorker::timerCallback()
{    
    float expression = processor->getExpressionValue();    
    
    sendMessage(expression);
}

void MidiOutWorker::sendMessage(float expression) {
    const int outputCCValue = 127.0 * expression;
    DBG(String("sending message ") + String(outputCCValue));
    MidiMessage message = MidiMessage::controllerEvent(1, 20, outputCCValue);
}

int MidiOutWorker::setMidiOutput(int index) {
    
    stopTimer();
    midiOutput = nullptr;

    if (MidiOutput::getDevices() [index].isNotEmpty())
    {
        midiOutput = MidiOutput::openDevice (index);
        jassert (midiOutput);
        MidiMessage message = MidiMessage::controllerEvent(1, 20, 99);
        DBG("sending message");
        midiOutput->sendMessageNow(message);
        
        startTimer(300);
        
        return index+1; // appropriate for ComboBox
    } 
    return 0;
}