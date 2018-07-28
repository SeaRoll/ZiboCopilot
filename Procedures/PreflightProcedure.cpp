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
#include "PreflightProcedure.h"


int PreflightProcedure::preflightProcedure(int stage)
{
	if (stage == 0) {	//YAW DAMPER
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[2])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
		return 1;
	}
	else if (stage == 1) { //NAVIGATION PANEL

		//VHF NAV SOURCE
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[3])) < 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/vhf_nav_source_rgt"));
		else if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[3])) > 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/vhf_nav_source_lft"));

		//IRS SOURCE
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[4])) < 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_source_right"));
		else if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[4])) > 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_source_left"));

		//FMC SOURCE
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[5])) < 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fmc_source_right"));
		else if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[5])) > 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fmc_source_left"));

		return 1;
	}
	else if (stage == 2) { //FUEL PUMPS
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[6])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[7])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[8])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[9])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
		return 1;
	}
	else if (stage == 3) { //CAB UTIL, PASS SEAT
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[10])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));

		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[11])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
		return 1;
	}
	else if (stage == 4) { //EMERGENCY LIGHTS
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[12])) < 1)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
		return 1;
	}
	else if (stage == 5) { //SEAT BELTS
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[13])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_dn"));
		return 1;
	}
	else if (stage == 6) { //NO SMOKING ALWAYS ON
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_dn"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_dn"));
		return 1;
	}
	else if (stage == 7) { //WINDOW HEAT
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[14])) == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
		}
		return 1;
	}
	else if (stage == 8) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[15])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[16])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
		return 1;
	}
	else if (stage == 9) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[17])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));

		return 1;
	}
	else if (stage == 10) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/l_pack_up"));
		return 1;
	}
	else if (stage == 11) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/iso_valve_dn"));
		return 1;
	}
	else if (stage == 12) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/r_pack_up"));
		return 1;
	}
	else if (stage == 13) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[18])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
		return 1;
	}
	else if (stage == 14) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[20])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_apu"));
		return 1;
	}
	else if (stage == 15) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[19])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
		return 1;
	}
	else if (stage == 16) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_on"));
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[21])) == 0)
			XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
		return 1;
	}
	else if (stage == 17) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/eng_start_source_right"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/eng_start_source_right"));
		return 1;
	}
	else if (stage == 18) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_flow_up"));
		return 1;
	}
	else if (stage == 19) {
		if (XPLMGetDatai(XPLMFindDataRef(DataRefList::dataRefList[22])) == 0)
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/autobrake_dn"));
		XPLMSpeakString("Preflight Procedures Completed");
		return 2;
	}
	return 0;
}
