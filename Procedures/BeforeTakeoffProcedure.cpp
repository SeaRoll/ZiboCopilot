#if APL
#if defined(__MACH__)
#include <Carbon/Carbon.h>
#endif
#endif
#include "XPLMMenus.h"
#include <cstdio>
#include <string>
#if IBM
#include <windows.h>
#endif
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMPlugin.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"
#include "XPWidgets.h"
#include "XPStandardWidgets.h"
#include "XPLMScenery.h"
#include "XPLMDataAccess.h"
#include <cstdlib>

using namespace std;

#include "DataRefList.h"
#include "BeforeTakeoffProcedure.h"

int BeforeTakeoffProcedure::beforeTakeoffProcedure(int stage)
{
	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
		return 1;
	}
	else if (stage == 1) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_on"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_off"));
		return 1;
	}
	else if (stage == 2) {
		XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_on"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
		return 1;
	}
	else if (stage == 3) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
		XPLMSpeakString("Before Takeoff Procedures Completed");
		return 2;
	}
	return 0;
}
