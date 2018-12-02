/*
  ==============================================================================

    NgimuComponent.cpp
    Created: 27 Feb 2018 9:52:09pm
    Author:  sigurds

  ==============================================================================
*/

#include "NgimuComponent.h"

//==============================================================================

NgimuComponent::NgimuComponent(OscBroadcaster *sender) : 
	OscComponent("ngimu"),
	sender(sender),
	battery(-1.0f),
	old_euler{ 0.0f, 0.0f, 0.0f },
	batteryStatus(500, 18, 100, 25)
{
	setFrameColour(Colours::green);
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
	
	String pattern = message.getAddressPattern().toString();
	if (pattern.startsWith("/battery")) {
		battery = message.isEmpty() ? -1.0f : message[0].getFloat32();
		MessageManager::callAsync([&]() {
			repaint(batteryStatus);
			});
	}
	else if (pattern.startsWith("/euler")) {
		float roll = message[ROLL].getFloat32();
		float pitch = message[PITCH].getFloat32();
		float yaw = message[YAW].getFloat32();
		if (abs(pitch) >= 85.0f) {	// Roll and pitch are unstable - use old values
			roll = old_euler[ROLL];
			old_euler[PITCH] = pitch;
			yaw = old_euler[YAW];
		}
		else {
			if (abs(roll - old_euler[ROLL]) > 320) {	// Must unwrap the roll value
				roll += (roll > old_euler[ROLL] ? -360 : 360);
			}
			if (abs(yaw - old_euler[YAW]) > 320) {	// Must unwrap the yaw value
				yaw += (yaw > old_euler[YAW] ? -360 : 360);
			}
			old_euler = { roll, pitch, yaw };
		}
		OSCAddressPattern address = getPrefix() + pattern;
		OSCMessage sendMessage(address, roll, pitch, yaw);
		sender->broadcastMessage(sendMessage);
	}
	else {
		OSCMessage sendMessage = message;
		OSCAddressPattern address = getPrefix() + pattern;
		sendMessage.setAddressPattern(address);
		sender->broadcastMessage(sendMessage);
	}
}
