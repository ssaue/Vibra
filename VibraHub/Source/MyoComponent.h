/*
  ==============================================================================

    MyoComponent.h
    Created: 13 Mar 2018 10:37:15pm
    Author:  sigurds

  ==============================================================================
*/

#pragma once

#include "OscComponent.h"

class MyoComponent : public OscComponent
{
public:

	MyoComponent(OscBroadcaster *sender);
	~MyoComponent();

private:
	void oscMessageReceived(const OSCMessage& message) override;

	OscBroadcaster *sender;
	String prefix;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyoComponent)
};
