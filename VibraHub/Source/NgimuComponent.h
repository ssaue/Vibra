/*
  ==============================================================================

    NgimuComponent.h
    Created: 27 Feb 2018 9:52:09pm
    Author:  sigurds

  ==============================================================================
*/

#pragma once

#include "OscComponent.h"

class NgimuComponent : public OscComponent
{
public:

	NgimuComponent(OscBroadcaster *sender, const String& id);
	~NgimuComponent();

	void paint(Graphics&) override;

private:
	void oscMessageReceived(const OSCMessage& message) override;

	OscBroadcaster *sender;
	String prefix;

	float battery;
	Rectangle<int> batteryStatus;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NgimuComponent)
};
