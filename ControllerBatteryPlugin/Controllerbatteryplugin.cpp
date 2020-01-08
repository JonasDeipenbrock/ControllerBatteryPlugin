// ControllerBatteryplugin.cpp : Defines the exported functions for the DLL application
//

#include "ControllerBatteryPlugin.h"
#include  "xinput.h"
#include <sstream>

BAKKESMOD_PLUGIN(ControllerBatteryPlugin, "Controller Battery Plugin", "0.1", 0)	//TODO: Update Plugin type


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
	cvarManager->registerCvar("cl_controllerbattery_display_status", "0", "Display status of the controller on the screen", true, 0, true, 1, true);
	cvarManager->registerCvar("cl_controllerbattery_shake_notification", "0", "Controller shake when becoming empty", true, 0, true, 0, true);

	//hook cvars

	cvarManager->registerNotifier("cl_controllerbattery_reset", [this](std::vector<string> params) {
		ResetStatus();
		}, "Resets Controller Status", PERMISSION_ALL);

	cvarManager->getCvar("cl_controllerbattery_shake_notification").addOnValueChanged(std::bind(&ControllerBatteryPlugin::logStatusToConsole, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->getCvar("cl_controllerbattery_display_status").addOnValueChanged(std::bind(&ControllerBatteryPlugin::logStatusToConsole, this, std::placeholders::_1, std::placeholders::_2));

}

//TODO implement
//cleanup old status
void ControllerBatteryPlugin::ResetStatus() {

}

void ControllerBatteryPlugin::Render(CanvasWrapper canvas)
{
	//check if status shall be drawn
	bool screenOut = cvarManager->getCvar("cl_controllerbattery_display_status").getBoolValue();
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
	Vector2 sz = canvas.GetSize();
	canvas.SetColor(0, 255, 0, 255);
}



void ControllerBatteryPlugin::logStatusToConsole(std::string oldValue, CVarWrapper cvar) {
	std::stringstream ss;
	ss << " cl_controllerbattery_shake_notification: " << cvarManager->getCvar("cl_controllerbattery_shake_notification").getBoolValue() << 
		" cl_controllerbattery_display_status:" << cvarManager->getCvar("cl_controllerbattery_display_status").getBoolValue();
	cvarManager->log(ss.str());
}



/*
	called when plugin is unload, cleanup
*/
void ControllerBatteryPlugin::onUnload()
{
}