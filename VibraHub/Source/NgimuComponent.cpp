/*
  ==============================================================================

    NgimuComponent.cpp
    Created: 27 Feb 2018 9:52:09pm
    Author:  sigurds

  ==============================================================================
*/

#include "NgimuComponent.h"

//==============================================================================

NgimuComponent::NgimuComponent(OscBroadcaster *sender, const String& id) :
	sender(sender),
	prefix("/ngimu/" + id),
	battery(-1.0f),
	batteryStatus(420, 18, 100, 25)
{
}

NgimuComponent::~NgimuComponent()
{
}

void NgimuComponent::paint(Graphics& g)
{
	OscComponent::paint(g);

	if (!isConnected()) {
		battery = -1.0f;
	}

	// Fill battery status with background color
	g.setColour(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	g.fillRoundedRectangle((float)batteryStatus.getX(), (float)batteryStatus.getY(),
		(float)batteryStatus.getWidth(), (float)batteryStatus.getHeight(), 5.0f);

	if (battery > 0.0) {
		Colour batteryColour = Colours::green;
		if (battery < 10.0f)
			batteryColour = Colours::red;
		else if (battery < 25.0f)
			batteryColour = Colours::orange;
		else if (battery < 50.0f)
			batteryColour = Colours::yellow;
		g.setColour(batteryColour);

		float batteryWidth = (float)batteryStatus.getWidth() * (battery / 100.0f);
		g.fillRoundedRectangle((float)batteryStatus.getX(), (float)batteryStatus.getY(),
			batteryWidth, (float)batteryStatus.getHeight(), 5.0f);
	}

	g.setColour(Colours::grey);
	g.drawRoundedRectangle((float)batteryStatus.getX(), (float)batteryStatus.getY(),
		(float)batteryStatus.getWidth(), (float)batteryStatus.getHeight(), 5.0f, 1);
	g.drawFittedText("Battery status", batteryStatus, Justification::centred, 1);
}

//==============================================================================
void NgimuComponent::oscMessageReceived(const OSCMessage& message)
{
	OscComponent::oscMessageReceived(message);
	
	OSCMessage sendMessage = message;
	String pattern = message.getAddressPattern().toString();
	if (pattern.startsWith("/battery")) {
		battery = message.isEmpty() ? -1.0f : message[0].getFloat32();
		MessageManager::callAsync([&]() {
			repaint(batteryStatus);
			});
	}
	else {
		OSCAddressPattern address = prefix + pattern;
		sendMessage.setAddressPattern(address);
		sender->broadcastMessage(sendMessage);
	}
}
