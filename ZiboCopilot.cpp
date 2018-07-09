//Created by Yohan Joo 18-06-21

#if APL
#if defined(__MACH__)
#include <Carbon/Carbon.h>
#endif
#endif
#include "XPLMMenus.h"
#include <stdio.h>
#include <string.h>
#if IBM
#include <windows.h>
#endif
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

bool powerUpProcedures = false;
bool preflightProcedures = false;
bool beforeTaxiProcedures = false;
bool beforeTakeoffProcedures = false;
bool cleanUpProcedures = false;
bool shutdownProcedures = false;

int ProcedureStage = 0;
float timeElapsed = 0;

static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);

int g_menu_container_idx; // The index of our menu item in the Plugins menu
XPLMMenuID g_menu_id; // The menu container we'll append all our menu items to
void menu_handler(void *, void *);


//-------------------------------------------------------- START OF PLUGIN -----------------------//
PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc)
{
	strcpy(outName, "ZiboCopilot");
	strcpy(outSig, "yohan.copilot.zibocopilot");
	strcpy(outDesc, "Copilot (like FS2Crew) for Zibo 737");

	g_menu_container_idx = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Copilot Menu", 0, 0);
	g_menu_id = XPLMCreateMenu("Copilot Menu", XPLMFindPluginsMenu(), g_menu_container_idx, menu_handler, NULL);
	XPLMAppendMenuItem(g_menu_id, "Powerup Procedure", (void *)"FlightProd_1", 1);
	XPLMAppendMenuItem(g_menu_id, "Preflight Procedures", (void *)"FlightProd_2", 1);
	XPLMAppendMenuItem(g_menu_id, "Before Taxi Procedures", (void *)"FlightProd_3", 1);
	XPLMAppendMenuItem(g_menu_id, "Before Takeoff Procedures", (void *)"FlightProd_4", 1);
	XPLMAppendMenuItem(g_menu_id, "Clean Up Procedures", (void *)"FlightProd_clean", 1);
	XPLMAppendMenuItem(g_menu_id, "Shutdown Procedures", (void *)"FlightProd_shutdown", 1);

	XPLMMenuID aircraft_menu = XPLMFindAircraftMenu();
	if (aircraft_menu) // This will be NULL unless this plugin was loaded with an aircraft (i.e., it was located in the current aircraft's "plugins" subdirectory)
	{
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Settings (Command-Based)", XPLMFindCommand("sim/operation/toggle_settings_window"));
	}

	return 1;
}


//-------------------------------------------------------- DO NOT TOUCH ------------------------------------------//
PLUGIN_API void    XPluginStop(void)
{
	// Since we created this menu, we'll be good citizens and clean it up as well
	XPLMDestroyMenu(g_menu_id);
	// If we were able to add a command to the aircraft menu, it will be automatically removed for us when we're unloaded
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }
//-------------------------------------------------------- END --------------------------------------------------//


// Handles all action from front-end
void menu_handler(void * in_menu_ref, void * in_item_ref)
{
	if (!strcmp((const char *)in_item_ref, "FlightProd_1"))
	{
		if(!powerUpProcedures) {
			XPLMSpeakString("Beginning Powerup Procedures");
			powerUpProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}else {
			powerUpProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_2"))
	{
		if (!preflightProcedures) {
			XPLMSpeakString("Beginning Preflight Procedures");
			preflightProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			preflightProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_3"))
	{
		if (!beforeTaxiProcedures) {
			XPLMSpeakString("Beginning Before Taxi Procedures");
			beforeTaxiProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			beforeTaxiProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_4"))
	{
		if (!beforeTakeoffProcedures) {
			XPLMSpeakString("Beginning Before Takeoff Procedures");
			beforeTakeoffProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			beforeTakeoffProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_clean"))
	{
		if (!cleanUpProcedures) {
			XPLMSpeakString("Beginning Clean Up Procedures");
			cleanUpProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			cleanUpProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_shutdown"))
	{
		if (!shutdownProcedures) {
			XPLMSpeakString("Beginning Shutdown Procedures");
			shutdownProcedures = true;
			timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			shutdownProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
}

//The loop for procedures
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon)
{
	/* Elapsed Time */
	float elapsed = XPLMGetElapsedTime();

	if (powerUpProcedures) {
		if (timeElapsed + 2 < elapsed && ProcedureStage == 0) { //BATTERY ON AND COVER
			XPLMCommandOnce(XPLMFindCommand("sim/electrical/battery_1_on"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover02"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 1) { //STANDBY POWER ON
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/standby_bat_on"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover03"));
			ProcedureStage++;
		}
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 2) { //GPU ON
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gpu_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 8 < elapsed && ProcedureStage == 3) { //APU
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 68 < elapsed && ProcedureStage == 4) { //APU GEN
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 70 < elapsed && ProcedureStage == 5) { //IRS ALIGN
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_R_right"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_R_right"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_L_right"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/irs_L_right"));
			XPLMSpeakString("Powerup Procedures Completed");
			powerUpProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
			ProcedureStage = 0;
		}
	}

	if (preflightProcedures) {
		if (timeElapsed + 2 < elapsed && ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_ctr1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_ctr2"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
			ProcedureStage++;
		}
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
			ProcedureStage++;
		}
		else if (timeElapsed + 8 < elapsed && ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 10 < elapsed && ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 12 < elapsed && ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
			ProcedureStage++;
		}
		else if (timeElapsed + 14 < elapsed && ProcedureStage == 7) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 16 < elapsed && ProcedureStage == 8) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));
			ProcedureStage++;
		}
		else if (timeElapsed + 18 < elapsed && ProcedureStage == 9) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/l_pack_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 19 < elapsed && ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/iso_valve_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 20 < elapsed && ProcedureStage == 11) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/r_pack_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 21 < elapsed && ProcedureStage == 12) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
			ProcedureStage++;
		}
		else if (timeElapsed + 22 < elapsed && ProcedureStage == 13) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_apu"));
			ProcedureStage++;
		}
		else if (timeElapsed + 23 < elapsed && ProcedureStage == 14) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 25 < elapsed && ProcedureStage == 15) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_on"));
			XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
			ProcedureStage++;
		}
		else if (timeElapsed + 26 < elapsed && ProcedureStage == 16) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/eng_start_source_right"));
			ProcedureStage++;
		}
		else if (timeElapsed + 28 < elapsed && ProcedureStage == 17) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_flow_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 29 < elapsed && ProcedureStage == 18) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/autobrake_dn"));
			ProcedureStage = 0;
			preflightProcedures = false;
			XPLMSpeakString("Preflight Procedures Completed");
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}

	if (beforeTaxiProcedures) {
		if (timeElapsed + 2 < elapsed && ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gen2_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 3 < elapsed && ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/gen1_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng2_start_right"));
			ProcedureStage++;
		}
		else if (timeElapsed + 5 < elapsed && ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng1_start_right"));
			ProcedureStage++;
		}
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 8 < elapsed && ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/l_pack_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 9 < elapsed && ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/iso_valve_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 10 < elapsed && ProcedureStage == 7) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/r_pack_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 11 < elapsed && ProcedureStage == 8) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_apu"));
			ProcedureStage++;
		}
		else if (timeElapsed + 13 < elapsed && ProcedureStage == 9) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fo_probes_pos"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/capt_probes_pos"));
			ProcedureStage++;
		}
		else if (timeElapsed + 15 < elapsed && ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/flaps_down"));
			ProcedureStage = 0;
			beforeTaxiProcedures = false;
			XPLMSpeakString("Before Taxi Procedures Completed");
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}

	if (beforeTakeoffProcedures) {
		if (timeElapsed + 1 < elapsed && ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			ProcedureStage++;
		}else if (timeElapsed + 2 < elapsed && ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_on"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_up"));
			ProcedureStage = 0;
			beforeTakeoffProcedures = false;
			XPLMSpeakString("Before Takeoff Procedures Completed");
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}

	if (cleanUpProcedures) {
		if (timeElapsed + 1 < elapsed && ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_down"));
			ProcedureStage++;
		}
		else if (timeElapsed + 2 < elapsed && ProcedureStage == 1) { //APU
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/spring_toggle_switch/APU_start_pos_dn"));
			ProcedureStage++;
		}
		else if (timeElapsed + 3 < elapsed && ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/wing_light_off"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
			XPLMCommandOnce(XPLMFindCommand("sim/flight_controls/speed_brakes_up_one"));
			ProcedureStage++;
		}
		else if (timeElapsed + 5 < elapsed && ProcedureStage == 4) {
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
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/transponder_mode_dn"));
			ProcedureStage = 0;
			cleanUpProcedures = false;
			XPLMSpeakString("Clean Up Procedures Completed");
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}

	if (shutdownProcedures) {
		if (timeElapsed + 2 < elapsed && ProcedureStage == 0) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen1_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/apu_gen2_dn"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture1_cutoff"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/engine/mixture2_cutoff"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/yaw_dumper"));
			ProcedureStage++;
		}
		else if (timeElapsed + 4 < elapsed && ProcedureStage == 1) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_lft2"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_ctr1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_ctr2"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fuel_pump_rgt2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 6 < elapsed && ProcedureStage == 2) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/cab_util_toggle"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/autopilot/ife_pass_seat_toggle"));
			ProcedureStage++;
		}
		else if (timeElapsed + 7 < elapsed && ProcedureStage == 3) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/button_switch_cover09"));
			ProcedureStage++;
		}
		else if (timeElapsed + 8 < elapsed && ProcedureStage == 4) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/seatbelt_sign_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 10 < elapsed && ProcedureStage == 5) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/no_smoking_up"));
			ProcedureStage++;
		}
		else if (timeElapsed + 12 < elapsed && ProcedureStage == 6) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_l_side"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_fwd"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/window_heat_r_side"));
			ProcedureStage++;
		}

		else if (timeElapsed + 13 < elapsed && ProcedureStage == 7) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/fo_probes_pos"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/capt_probes_pos"));
			ProcedureStage++;
		}
		else if (timeElapsed + 14 < elapsed && ProcedureStage == 8) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps1"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/electric_hydro_pumps2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 16 < elapsed && ProcedureStage == 9) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/trim_air"));
			ProcedureStage++;
		}
		else if (timeElapsed + 17 < elapsed && ProcedureStage == 10) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_1"));
			ProcedureStage++;
		}
		else if (timeElapsed + 18 < elapsed && ProcedureStage == 11) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/bleed_air_2"));
			ProcedureStage++;
		}
		else if (timeElapsed + 19 < elapsed && ProcedureStage == 12) {
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/toggle_switch/position_light_up"));
			XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/logo_light_off"));
			XPLMCommandOnce(XPLMFindCommand("sim/lights/beacon_lights_toggle"));
			ProcedureStage = 0;
			shutdownProcedures = false;
			XPLMSpeakString("Shutdown Procedures Completed");
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}

	/* Return 1.0 to indicate that we want to be called again in 1 second. */
	return 1.0;
}
