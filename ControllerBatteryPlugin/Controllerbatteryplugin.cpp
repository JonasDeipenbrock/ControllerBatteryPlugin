// ControllerBatteryplugin.cpp : Defines the exported functions for the DLL application
//

#include "ControllerBatteryPlugin.h"
#include  "xinput.h"
#include <sstream>

BAKKESMOD_PLUGIN(ControllerBatteryPlugin, "Controller Battery Plugin", "0.1", 0)	//TODO: Update Plugin type
float refreshIntervall = 15;
XINPUT_VIBRATION vibration;
DWORD current_user = 0;

/*
	called when plugin is loaded, main command here
*/
void ControllerBatteryPlugin::onLoad()
{
	// log startup to console
	std::stringstream ss;
	ss << exports.pluginName << " version: " << exports.pluginVersion;
	cvarManager->log(ss.str());


	//create cvars
	cvarManager->registerCvar("controllerbattery_display_status", "0", "Display status of the controller on the screen", true, 0, true, 1, true);
	cvarManager->registerCvar("controllerbattery_shake_notification", "0", "Controller shake when becoming empty", true, 0, true, 1, true);
	cvarManager->registerCvar("controllerbattery_window_position_x", "0", "Current x Position of the window", true, true, 0, true, 1920, true);
	cvarManager->registerCvar("controllerbattery_window_position_y", "0", "Current y Position of the window", true, true, 0, true, 1080, true);
	

	//hook cvars
	cvarManager->registerNotifier("controllerbattery_reset", [this](std::vector<string> params) {
		ResetStatus();
		}, "Resets Controller Status", PERMISSION_ALL);
	
	cvarManager->getCvar("controllerbattery_shake_notification").addOnValueChanged(std::bind(&ControllerBatteryPlugin::logStatusToConsole, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->getCvar("controllerbattery_display_status").addOnValueChanged(std::bind(&ControllerBatteryPlugin::logStatusToConsole, this, std::placeholders::_1, std::placeholders::_2));

	// remove later
	//Test vibration
	this->setVibration(32000, 16000);
	this->vibrate();

	//init state
	/*
		Get init value and reset old values
		*/

	//hook events
	gameWrapper->RegisterDrawable(std::bind(&ControllerBatteryPlugin::Render, this, std::placeholders::_1));
}

//TODO implement
//cleanup old status
void ControllerBatteryPlugin::ResetStatus() {

}

void ControllerBatteryPlugin::updateStatus() {
	XInputGetBatteryInformation;
		//check if plugin enabled

		//check if controlled connected over blue

		//set 
	//loop
	this->gameWrapper->SetTimeout([this](GameWrapper* gw) {updateStatus();}, refreshIntervall);
}

void ControllerBatteryPlugin::Render(CanvasWrapper canvas)
{


	//check if status shall be drawn
	bool screenOut = cvarManager->getCvar("controllerbattery_display_status").getBoolValue();
	if (!(screenOut))
		return;

	//unsure what this does rn
	static int count = 0, delta = 1;
#define STEP 5
	count += STEP;
	if (count < STEP)
		delta = 1;
	if (count > (255 - STEP))
		delta = -STEP;
	byte connect = status.connected;
	//check if connected and draw unconnected to screen

	//get current battery level
	byte level = status.batteryLevel;
#define SPACER 200
	//starting values window
	Vector2 sz = canvas.GetSize();

	
	int pos_x = cvarManager->getCvar("controllerbattery_window_position_x").getIntValue();
	int pos_y = cvarManager->getCvar("controllerbattery_window_position_y").getIntValue();

	/**
	//testprint to console
	std::stringstream ss;
	ss << "current x: " << position_x << "current y: " << position_y;
	cvarManager->log(ss.str());	*/


	//loop that updates window size etc.
	canvas.SetColor(0, 255, 0, 255);
	canvas.SetPosition(Vector2{ pos_x, sz.Y - 300 });
	canvas.DrawString("TestString" + std::to_string(15), 3, 3);
}


void ControllerBatteryPlugin::setVibration(int left, int right) {
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
}


void ControllerBatteryPlugin::vibrate() {
	XInputSetState(current_user, &vibration);
	setVibration(0, 0);
	XInputSetState(current_user, &vibration);
}


void ControllerBatteryPlugin::logStatusToConsole(std::string oldValue, CVarWrapper cvar) {
	std::stringstream ss;
	ss << "controllerbattery_shake_notification: " << cvarManager->getCvar("controllerbattery_shake_notification").getBoolValue() << 
		" controllerbattery_display_status:" << cvarManager->getCvar("controllerbattery_display_status").getBoolValue();
	cvarManager->log(ss.str());
}



/*
	called when plugin is unload, cleanup
*/
void ControllerBatteryPlugin::onUnload()
{
}