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
    midiOutputList->addChangeListener(this);
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


void MidiOutWorker::setMidiOutput(const String& midiOutName) {
    //const StringArray midiOuts = MidiOutput::getDevices();
    const StringArray midiOuts = midiOutputList->getOutputList();
    /*
    const int indexOfRequestedOutput = midiOuts.indexOf(midiOutName);
    if(indexOfRequestedOutput != -1) {
        // setMidiOutput
    }
    return indexOfRequestedOutput;
    */
    midiOutput = MidiOutput::openDevice(midiOuts.indexOf(midiOutName));
    sendChangeMessage();
}

void MidiOutWorker::stop() {
    stopTimer();
}

const String& MidiOutWorker::getMidiOutputName() {
    if(midiOutput==nullptr)
        return "";
    return midiOutput->getName();
}


// use this for alert from MidiOutputList that devices have changed, and pass up to 
// own MidiOutputComboBox via sendChangeMessage()
void MidiOutWorker::changeListenerCallback(ChangeBroadcaster* cb) {
    DBG("MidiOutWorker::changeListenerCallback");
    if(cb == midiOutputList)
    {
        setMidiOutput(getMidiOutputName());
    }
}

// shameless assumption
void MidiOutWorker::comboBoxChanged(ComboBox* src) {
    DBG("MidiOutWorker::comboBoxChanged");
    setMidiOutput(src->getText());
}


MidiOutputList::MidiOutputList()
{
    currentOutputList = MidiOutput::getDevices();
    startTimer(500);
}

MidiOutputList::~MidiOutputList() {}

void MidiOutputList::timerCallback() {
    if(hasOutputListChanged()) {
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

int MidiOutputList::indexOf(const String& name)
{
    return currentOutputList.indexOf(name);
}
