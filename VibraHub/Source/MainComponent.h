#pragma once

#include "OscComponent.h"

class NgimuComponent;

class MainComponent   : public OscComponent, public OscBroadcaster
{
public:

	MainComponent();
	~MainComponent();

	void paint(Graphics& g);
	void resized();

private:
	void oscMessageReceived(const OSCMessage& message) override;
	void broadcastMessage(const OSCMessage& message) override;

	void connectListener(const String& address);
	void disconnectListener(const String& address);

	ScopedPointer<Label> sendLabel;
	ScopedPointer<Label> sendField;

	ScopedPointer<NgimuComponent> ngimu1;
    ScopedPointer<NgimuComponent> ngimu2;
    ScopedPointer<NgimuComponent> ngimu3;

	Array<String> addresses;
	OSCSender sender;

	int sendPortNumber;

	//==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
