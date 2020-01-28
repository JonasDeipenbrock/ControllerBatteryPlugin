#pragma once
#pragma comment( lib, "bakkesmod.lib")


#include "bakkesmod/plugin/bakkesmodplugin.h"
#include  "xinput.h"



typedef struct {
	boolean connected;
	byte batteryLevel;
} BatteryStatusStruct;


class ControllerBatteryPlugin : public BakkesMod::Plugin::BakkesModPlugin
{

private: 
	BatteryStatusStruct status;
	float refreshTime;


	void logStatusToConsole(std::string oldValue, CVarWrapper cvar);
	void updateStatus();
public:
	virtual void onLoad();
	virtual void onUnload();

	void Render(CanvasWrapper canvas);
	void setVibration(int left, int right);
	void vibrate();
	void ResetStatus();
};