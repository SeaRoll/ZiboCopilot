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

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

#include "SubHandler.h"

void SubHandler::finishProcedure(int id)
{
	procedures[id] = false;
	ProcedureStage = 0;
	ProcedureType++;
	doneProcedures = true;
}


void SubHandler::doProcedures(float elapsed) {
	if (procedures[power_up]) {
		if (ProcedureStage == 0) { //BATTERY ON AND COVER
			XPLMCommandOnce(XPLMFindCommand("sim/electrical/battery_1_on"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover02"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) { //STANDBY POWER ON
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/standby_bat_on"));
			//XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover03"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 2) { //GPU ON
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gpu_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) { //APU
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 4)
		{
			XPLMCommandBegin(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 5) // APU BUTTON RELEASE
		{
			XPLMCommandEnd(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 6 && XPLMGetDatai(XPLMFindDataRef(dataRefList[25])) == 1) { //APU GEN
			XPLMCommandBegin(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
			XPLMCommandBegin(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 7)
		{
			XPLMCommandEnd(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
			XPLMCommandEnd(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 8) { //IRS ALIGN

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[1])) < 2) {
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_R_right"));
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_R_right"));
			}

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[0])) < 2) {
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_L_right"));
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_L_right"));
			}

			XPLMSpeakString("Powerup Procedures Completed");
			finishProcedure(power_up);
		}
	}

	if (procedures[pre_flight]) {
		if (ProcedureStage == 0) {	//YAW DAMPER
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[2])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) { //NAVIGATION PANEL

			//VHF NAV SOURCE
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[3])) < 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/vhf_nav_source_rgt"));
			else if (XPLMGetDatai(XPLMFindDataRef(dataRefList[3])) > 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/vhf_nav_source_lft"));

			//IRS SOURCE
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[4])) < 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_source_right"));
			else if (XPLMGetDatai(XPLMFindDataRef(dataRefList[4])) > 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_source_left"));

			//FMC SOURCE
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[5])) < 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fmc_source_right"));
			else if (XPLMGetDatai(XPLMFindDataRef(dataRefList[5])) > 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fmc_source_left"));

			ProcedureStage++;
		}
		else if (ProcedureStage == 2) { //FUEL PUMPS
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[6])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[7])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[8])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[9])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) { //CAB UTIL, PASS SEAT
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[10])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[11])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 4) { //EMERGENCY LIGHTS
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[12])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 5) { //SEAT BELTS
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[13])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 6) { //NO SMOKING ALWAYS ON
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 7) { //WINDOW HEAT
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[14])) == 0) {
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
			}
			ProcedureStage++;
		}
		else if (ProcedureStage == 8) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[15])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[16])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 9) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[17])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));

			ProcedureStage++;
		}
		else if (ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/l_pack_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 11) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/iso_valve_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 12) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/r_pack_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 13) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[18])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 14) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[20])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_apu"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 15) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[19])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 16) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_on"));
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[21])) == 0)
				XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 17) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/eng_start_source_right"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/eng_start_source_right"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 18) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_flow_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 19) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[22])) == 0)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/autobrake_dn"));
			XPLMSpeakString("Preflight Procedures Completed");
			finishProcedure(pre_flight);

		}
	}

	if (procedures[before_taxi]) {
		if (ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gen2_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gen1_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng2_start_cont"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng1_start_cont"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/l_pack_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/iso_valve_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 7) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/r_pack_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 8) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_apu"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 9) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[24])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fo_probes_pos"));
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[23])) < 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/capt_probes_pos"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_left_on"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_right_on"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 11) {
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			XPLMSpeakString("Before Taxi Procedures Completed");
			finishProcedure(before_taxi);
		}
	}

	if (procedures[before_take_off]) {
		if (ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_on"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_on"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMSpeakString("Before Takeoff Procedures Completed");
			finishProcedure(before_take_off);
		}
	}

	if (procedures[clean_up]) {
		if (ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) { //APU
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_off"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_up"));

			ProcedureStage++;
		}
		else if (ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_off"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMSpeakString("Clean Up Procedures Completed");
			finishProcedure(clean_up);
		}
	}

	if (procedures[shutdown]) {
		if (ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/taxi_light_brightness_pos_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_left_off"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_right_off"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture1_cutoff"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture2_cutoff"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 2) {
			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[6])) == 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[7])) == 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[8])) == 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));

			if (XPLMGetDatai(XPLMFindDataRef(dataRefList[9])) == 1)
				XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_up"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 7) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
			ProcedureStage++;
		}

		else if (ProcedureStage == 8) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fo_probes_pos"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/capt_probes_pos"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 9) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 11) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 12) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 13) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng1_start_off"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/rotary/eng2_start_off"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 14) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_off"));
			XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 15)
		{
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover02"));
			ProcedureStage++;
		}
		else if (ProcedureStage == 16)
		{
			XPLMCommandOnce(XPLMFindCommand("sim/electrical/battery_1_off"));
			XPLMSpeakString("Shutdown Procedures Completed");
			finishProcedure(shutdown);
			ProcedureType = 0;
		}
	}
}