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
	ngimu1(new NgimuComponent(this, String("1"))),
	ngimu2(new NgimuComponent(this, String("2"))),
	ngimu3(new NgimuComponent(this, String("3"))),
	myo1(new MyoComponent(this)),
	myo2(new MyoComponent(this)),
	myo3(new MyoComponent(this)),
	myo4(new MyoComponent(this))
{
	setSize(700, 560);

	ngimu1->setBounds(0, 70, 700, 130);
	addAndMakeVisible(ngimu1);

	ngimu2->setBounds(0, 140, 700, 200);
	addAndMakeVisible(ngimu2);

	ngimu3->setBounds(0, 210, 700, 270);
	addAndMakeVisible(ngimu3);

	myo1->setBounds(0, 280, 700, 340);
	addAndMakeVisible(myo1);

	myo2->setBounds(0, 350, 700, 410);
	addAndMakeVisible(myo2);

	myo3->setBounds(0, 420, 700, 480);
	addAndMakeVisible(myo3);

	myo4->setBounds(0, 490, 700, 550);
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


