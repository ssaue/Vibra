/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "NgimuComponent.h"

static const OSCAddress connectPattern = "/vibra/connect";
static const OSCAddress disconnectPattern = "/vibra/disconnect";

//==============================================================================
MainComponent::MainComponent() : 
	sendLabel(new Label),
	sendField(new Label),
	ngimu1(new NgimuComponent(this, String("1"))),
	ngimu2(new NgimuComponent(this, String("2"))),
	ngimu3(new NgimuComponent(this, String("3")))
{
	setSize(700, 400);

	sendLabel->setText("UDP Send Port: ", dontSendNotification);
	sendLabel->setBounds(420, 18, 130, 25);
	addAndMakeVisible(sendLabel);

	sendField->setText("12000", dontSendNotification);
	sendField->setEditable(true, true, true);
	sendField->setBounds(550, 18, 50, 25);
	addAndMakeVisible(sendField);

	ngimu1->setBounds(0, 70, 700, 130);
	addAndMakeVisible(ngimu1);

	ngimu2->setBounds(0, 140, 700, 200);
	addAndMakeVisible(ngimu2);

	ngimu3->setBounds(0, 210, 700, 270);
	addAndMakeVisible(ngimu3);
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
		if (!message.isEmpty()) {
			connectListener(message[0].getString());
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
	for (auto& element : addresses) {
		sender.sendToIPAddress(element, sendPortNumber, message);
	}
}

void MainComponent::connectListener(const String& address) {
	addresses.addIfNotAlreadyThere(address);
}

void MainComponent::disconnectListener(const String& address) {
	addresses.removeFirstMatchingValue(address);
}


