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
    if(!evs->isActive()) return;
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

    if (index != -1 && MidiOutput::getDevices() [index].isNotEmpty())
    {
        midiOutput = MidiOutput::openDevice (index);
        jassert (midiOutput);
        
        startTimer(30);
        
        return index;
    } 
    return -1;
}


int MidiOutWorker::setMidiOutput(const String& midiOutName) {
    const StringArray midiOuts = MidiOutput::getDevices();
    /*
    const int indexOfRequestedOutput = midiOuts.indexOf(midiOutName);
    if(indexOfRequestedOutput != -1) {
        // setMidiOutput
    }
    return indexOfRequestedOutput;
    */
    return setMidiOutput(midiOuts.indexOf(midiOutName));
}

void MidiOutWorker::stop() {
    stopTimer();
}

const String& MidiOutWorker::getMidiOutputName() {
    return midiOutput->getName();
}





MidiOutputList::MidiOutputList()
{
    currentOutputList = MidiOutput::getDevices();
    startTimer(500);
}

MidiOutputList::~MidiOutputList() {}

void MidiOutputList::timerCallback() {
    if(hasOutputListChanged()) {
        // maybe midiOutWorker should be handled from here,
        // with the results broadcast by change messages
        
        sendChangeMessage();
    }
}

bool MidiOutputList::hasOutputListChanged() {
    const StringArray oldOutputList = currentOutputList;
    currentOutputList = MidiOutput::getDevices();
    
    if(oldOutputList.size() != currentOutputList.size()) return true;

    
    for (int i = 0; i < currentOutputList.size(); ++i)
        if (currentOutputList[i] != oldOutputList[i])
            return true;

    return false;
    
}