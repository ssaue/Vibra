/*
  ==============================================================================

    MyoComponent.cpp
    Created: 13 Mar 2018 10:37:15pm
    Author:  sigurds

  ==============================================================================
*/

#include "MyoComponent.h"

MyoComponent::MyoComponent(OscBroadcaster *sender) :
	OscComponent("myo"),
	sender(sender)
{
	setFrameColour(Colours::red);
}

MyoComponent::~MyoComponent()
{
}

//==============================================================================
void MyoComponent::oscMessageReceived(const OSCMessage& message)
{
	OscComponent::oscMessageReceived(message);

	OSCMessage sendMessage = message;
	String pattern = message.getAddressPattern().toString();
	if (pattern.startsWith("/myo")) {
		OSCAddressPattern address = getPrefix() + pattern.substring(5);
		sendMessage.setAddressPattern(address);
		sender->broadcastMessage(sendMessage);
	}
}
