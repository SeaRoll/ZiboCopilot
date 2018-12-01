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
#include "CleanupProcedure.h"

int CleanupProcedure::cleanupProcedure(int stage)
{
	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
		return 1;
	}
	else if (stage == 1) { //APU
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
		return 1;
	}
	else if (stage == 2) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_off"));
		return 1;
	}
	else if (stage == 3) {
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
		return 1;
	}
	else if (stage == 4) {
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
		return 1;
	}
	else if (stage == 5) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
		XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_off"));
		return 1;
	}
	else if (stage == 6) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
		XPLMSpeakString("Clean Up Procedures Completed");
		return 2;
	}
	return 0;
}
