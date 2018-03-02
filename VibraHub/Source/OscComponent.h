/*
  ==============================================================================

    OscComponent.h
    Created: 1 Mar 2018 9:16:09pm
    Author:  sigurds

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OscBroadcaster {
public:
	virtual void broadcastMessage(const OSCMessage& message) = 0;
};

class OscComponent    : public Component,
	private Button::Listener,
	private OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
    OscComponent();
    ~OscComponent();

    void paint (Graphics&) override;
    void resized() override;

protected:
	void oscMessageReceived(const OSCMessage& message) override;
	bool isConnected();

private:
	ScopedPointer<Label> portNumberLabel;
	ScopedPointer<Label> portNumberField;
	ScopedPointer<TextButton> connectButton;
	
	ScopedPointer<OSCReceiver> oscReceiver;

	int currentPortNumber;
	bool messageReceived;

	Rectangle<int> connectionStatus;
	Rectangle<int> activityStatus;

	//==============================================================================

	void buttonClicked(Button* b) override;
	void connectButtonClicked();

	void oscBundleReceived(const OSCBundle& bundle) override;

	void connect();
	void disconnect();

	void handleConnectError(int failedPort);
	void handleDisconnectError();

	void handleInvalidPortNumberEntered();
	void handleInvalidOSCPacket(const char* data, int dataSize);

	bool isActive();
	bool isValidOscPort(int port);

	//==============================================================================
		
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
