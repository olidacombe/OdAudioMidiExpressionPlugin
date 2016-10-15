/*
  ==============================================================================

    MidiOutWorker.cpp
    Created: 12 Oct 2016 2:30:10pm
    Author:  Oli

  ==============================================================================
*/

#include "MidiOutWorker.h"
//#include "PluginProcessor.h"

#include "../JuceLibraryCode/JuceHeader.h"

MidiOutWorker::MidiOutWorker(ExpressionValueSource* e)
:
    //Thread("Midi Output Worker"),
    evs(e),
    lastOutputCCValue(0), currentOutputCCValue(0)
{

}

MidiOutWorker::~MidiOutWorker()
{
    //stopThread(2000);
    
}

void MidiOutWorker::timerCallback()
{    
    float expression = evs->getExpressionValue();    
    currentOutputCCValue = jmin(127.0 * expression, 127.0);
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
        
        startTimer(30);
        
        return index;
    } 
    return 0;
}

int MidiOutWorker::setMidiOutput(const String& midiOutName) {
    return 0;
}

const String& MidiOutWorker::getMidiOutputName() {
    return midiOutput->getName();
}