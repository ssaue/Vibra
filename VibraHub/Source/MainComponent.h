#pragma once

#include "OscComponent.h"
#include <map>

class NgimuComponent;
class MyoComponent;

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

	void connectListener(const String& address, const int port);
	void disconnectListener(const String& address);

	ScopedPointer<NgimuComponent> ngimu1;
    ScopedPointer<NgimuComponent> ngimu2;
    ScopedPointer<NgimuComponent> ngimu3;
	ScopedPointer<NgimuComponent> ngimu4;

	ScopedPointer<MyoComponent> myo1;
	ScopedPointer<MyoComponent> myo2;
	ScopedPointer<MyoComponent> myo3;
	ScopedPointer<MyoComponent> myo4;

	std::map<String, ScopedPointer<OSCSender>> senders;

	//==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
