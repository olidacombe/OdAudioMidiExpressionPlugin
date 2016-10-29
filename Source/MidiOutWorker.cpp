/*
  ==============================================================================

    MidiOutWorker.cpp
    Created: 12 Oct 2016 2:30:10pm
    Author:  Oli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiOutWorker.h"
#include "ExpressionValueMachine.h"

MidiOutWorker::MidiOutWorker(ExpressionValueSource* e)
: lastOutputCCValue(0), currentOutputCCValue(0), evs(e)
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

// change this to not call MidiOutput::getDevices and try directly
// from what is held in midiOutput object.  We'll be re-run when that
// detects a change anyhow.
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


// use this for alert from MidiOutputList that devices have changed, and pass up to 
// own MidiOutputComboBox via sendChangeMessage()
void MidiOutWorker::changeListenerCallback(ChangeBroadcaster* cb) {
    
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
