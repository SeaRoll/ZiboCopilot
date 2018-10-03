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
#include "ShutdownProcedure.h"

int ShutdownProcedure::shutdownProcedure(int stage)
{
	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_left_off"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_right_off"));
		return 1;
	}
	else if (stage == 1) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture1_cutoff"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture2_cutoff"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
		return 1;
	}
	else if (stage == 2) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[6])) == 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[7])) == 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[8])) == 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[9])) == 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
		return 1;
	}
	else if (stage == 3) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
		return 1;
	}
	else if (stage == 4) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
		return 1;
	}
	else if (stage == 5) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_up"));
		return 1;
	}
	else if (stage == 6) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_up"));
		return 1;
	}
	else if (stage == 7) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
		return 1;
	}

	else if (stage == 8) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fo_probes_pos"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/capt_probes_pos"));
		return 1;
	}
	else if (stage == 9) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
		return 1;
	}
	else if (stage == 10) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));
		return 1;
	}
	else if (stage == 11) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
		return 1;
	}
	else if (stage == 12) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
		return 1;
	}
	else if (stage == 13) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng1_start_off"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng2_start_off"));
		return 1;
	}
	else if (stage == 14) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_off"));
		XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
		return 1;
	}
	else if (stage == 15)
	{
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover02"));
		XPLMSpeakString("Shutdown Procedures Completed");
		return 2;
	}
	return 0;
}
