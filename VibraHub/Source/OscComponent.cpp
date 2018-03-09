/*
  ==============================================================================

    OscComponent.cpp
    Created: 1 Mar 2018 9:16:09pm
    Author:  sigurds

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent() :
	portNumberLabel(new Label),
	portNumberField(new Label),
	connectButton(new TextButton("Connect")),
	oscReceiver(new OSCReceiver),
	currentPortNumber(-1),
	messageReceived(false),
	connectionStatus(340, 18, 25, 25),
	activityStatus(380, 18, 25, 25)
{
	portNumberLabel->setText("UDP Port Number: ", dontSendNotification);
	portNumberLabel->setBounds(10, 18, 130, 25);
	addAndMakeVisible(portNumberLabel);

	portNumberField->setText("9001", dontSendNotification);
	portNumberField->setEditable(true, true, true);
	portNumberField->setBounds(140, 18, 50, 25);
	addAndMakeVisible(portNumberField);

	connectButton->setBounds(210, 18, 100, 25);
	addAndMakeVisible(connectButton);
	connectButton->addListener(this);

	oscReceiver->addListener(this);
	oscReceiver->registerFormatErrorHandler(
		[this](const char* data, int dataSize)
	{
		handleInvalidOSCPacket(data, dataSize);
	}
	);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (Graphics& g)
{
	Colour background = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll (background);   // clear the background

	// Draw a rectangle for connection status
	g.setColour(isConnected() ? Colours::green : Colours::red);
	g.fillRoundedRectangle((float) connectionStatus.getX(), (float) connectionStatus.getY(), 
		(float) connectionStatus.getWidth(), (float) connectionStatus.getHeight(), 5.0f);

	// Draw a rectangle for activity status
	g.setColour(isActive() ? Colours::green : background);
	g.fillRoundedRectangle((float) activityStatus.getX(), (float) activityStatus.getY(), 
		(float) activityStatus.getWidth(), (float) activityStatus.getHeight(), 5.0f);

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void OscComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


//==============================================================================
void OscComponent::buttonClicked(Button*)
{
	connectButtonClicked();
}

//==============================================================================
void OscComponent::connectButtonClicked()
{
	if (!isConnected())
		connect();
	else
		disconnect();

	repaint();
}

//==============================================================================
void OscComponent::oscMessageReceived(const OSCMessage& /*message*/)
{
	if (!messageReceived) {
		messageReceived = true;
		MessageManager::callAsync([&]() {
			repaint(activityStatus); 
		});
	}
}

void OscComponent::oscBundleReceived(const OSCBundle& bundle)
{
	for (auto& element : bundle)
	{
		if (element.isMessage())
			oscMessageReceived(element.getMessage());
		else if (element.isBundle())
			oscBundleReceived(element.getBundle());
	}
}

//==============================================================================
void OscComponent::connect()
{
	int portToConnect = portNumberField->getText().getIntValue();

	if (!isValidOscPort(portToConnect))
	{
		handleInvalidPortNumberEntered();
		return;
	}

	if (oscReceiver->connect(portToConnect))
	{
		currentPortNumber = portToConnect;
		connectButton->setButtonText("Disconnect");
	}
	else
	{
		handleConnectError(portToConnect);
	}
}

//==============================================================================
void OscComponent::disconnect()
{
	if (oscReceiver->disconnect())
	{
		currentPortNumber = -1;
		messageReceived = false;
		connectButton->setButtonText("Connect");
	}
	else
	{
		handleDisconnectError();
	}
}

//==============================================================================
void OscComponent::handleConnectError(int failedPort)
{
	AlertWindow::showMessageBoxAsync(
		AlertWindow::WarningIcon,
		"OSC Connection error",
		"Error: could not connect to port " + String(failedPort),
		"OK");
}

//==============================================================================
void OscComponent::handleDisconnectError()
{
	AlertWindow::showMessageBoxAsync(
		AlertWindow::WarningIcon,
		"Unknown error",
		"An unknown error occured while trying to disconnect from UPD port.",
		"OK");
}

//==============================================================================
void OscComponent::handleInvalidPortNumberEntered()
{
	AlertWindow::showMessageBoxAsync(
		AlertWindow::WarningIcon,
		"Invalid port number",
		"Error: you have entered an invalid UDP port number.",
		"OK");
}

void OscComponent::handleInvalidOSCPacket(const char*, int)
{
}

//==============================================================================
bool OscComponent::isConnected()
{
	return currentPortNumber != -1;
}

bool OscComponent::isActive()
{
	return messageReceived;
}

//==============================================================================
bool OscComponent::isValidOscPort(int port)
{
	return port > 0 && port < 65536;
}

//==============================================================================
