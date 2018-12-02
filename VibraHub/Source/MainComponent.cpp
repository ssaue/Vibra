/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "NgimuComponent.h"
#include "MyoComponent.h"

static const OSCAddress connectPattern = "/vibra/connect";
static const OSCAddress disconnectPattern = "/vibra/disconnect";

//==============================================================================
MainComponent::MainComponent() : 
	OscComponent("Main"),
	ngimu1(new NgimuComponent(this)),
	ngimu2(new NgimuComponent(this)),
	ngimu3(new NgimuComponent(this)),
	ngimu4(new NgimuComponent(this)),
	myo1(new MyoComponent(this)),
	myo2(new MyoComponent(this)),
	myo3(new MyoComponent(this)),
	myo4(new MyoComponent(this))
{
	setSize(700, 630);

	ngimu1->setBounds(0, 70, 700, 130);
	ngimu1->setID(1);
	addAndMakeVisible(ngimu1);

	ngimu2->setBounds(0, 140, 700, 200);
	ngimu2->setID(2);
	addAndMakeVisible(ngimu2);

	ngimu3->setBounds(0, 210, 700, 270);
	ngimu3->setID(3);
	addAndMakeVisible(ngimu3);

	ngimu4->setBounds(0, 280, 700, 340);
	ngimu4->setID(4);
	addAndMakeVisible(ngimu4);

	myo1->setBounds(0, 350, 700, 410);
	myo1->setID(1);
	addAndMakeVisible(myo1);

	myo2->setBounds(0, 420, 700, 480);
	myo2->setID(2);
	addAndMakeVisible(myo2);

	myo3->setBounds(0, 490, 700, 550);
	myo3->setID(3);
	addAndMakeVisible(myo3);

	myo4->setBounds(0, 560, 700, 620);
	myo4->setID(4);
	addAndMakeVisible(myo4);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
	OscComponent::paint(g);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::oscMessageReceived(const OSCMessage& message)
{
	OscComponent::oscMessageReceived(message);
	const OSCAddressPattern& address = message.getAddressPattern();

	if (address.matches(connectPattern)) {
		if (message.size() >= 2 && message[0].isString () && message[1].isInt32()) {
			connectListener(message[0].getString(), message[1].getInt32());
		}
	}
	else if (address.matches(disconnectPattern)) {
		if (!message.isEmpty()) {
			disconnectListener(message[0].getString());
		}
	}
	else {
		broadcastMessage(message);
	}
}


void MainComponent::broadcastMessage(const OSCMessage& message) {
	for (auto& element : senders) {
		element.second->send(message);
	}
}

void MainComponent::connectListener(const String& address, const int port) {
	ScopedPointer<OSCSender> new_sender = new OSCSender();
	if (new_sender->connect(address, port)) {
		senders[address] = new_sender;
	}
}

void MainComponent::disconnectListener(const String& address) {
	senders.erase(address);
}


