#pragma once
#ifndef SubHandler_H
#define SubHandler_H

#if APL
#if defined(__MACH__)
#include <Carbon/Carbon.h>
#endif
#endif
#include "XPLMMenus.h"
#include <stdio.h>
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
#include <stdlib.h>

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

using namespace std;

class SubHandler
{
public:

	const char dataRefList[100][255] = {
		"laminar/B738/toggle_switch/irs_left",						//IRS LEFT 0
		"laminar/B738/toggle_switch/irs_right",						//IRS RIGHT 1
		"sim/cockpit/switches/yaw_damper_on",						//IS YAW DAMPER ON? 2
		"laminar/B738/toggle_switch/vhf_nav_source",				//VHF NAV SOURCE 3
		"laminar/B738/toggle_switch/irs_source",					//IRS SOURCE 4
		"laminar/B738/toggle_switch/fmc_source",					//FMC SOURCE 5
		"laminar/B738/fuel/fuel_tank_pos_lft1",						//FUEL PUMP 1 6
		"laminar/B738/fuel/fuel_tank_pos_lft2",						//FUEL PUMP 2 7
		"laminar/B738/fuel/fuel_tank_pos_rgt1",						//FUEL PUMP 3 8
		"laminar/B738/fuel/fuel_tank_pos_rgt2",						//FUEL PUMP 4 9
		"laminar/B738/toggle_switch/cab_util_pos",					//CAB UTIL 10
		"laminar/B738/toggle_switch/ife_pass_seat_pos",				//IFE PASS SEAT 11
		"laminar/B738/toggle_switch/emer_exit_lights",				//EMERGENCY EXIT LIGHTS 12
		"laminar/B738/toggle_switch/seatbelt_sign_pos",				//SEATBELT SIGNS 13
		"laminar/B738/ice/window_heat_l_side_pos",					//CHECK WINDOW HEAT 14
		"laminar/B738/toggle_switch/electric_hydro_pumps1_pos",		//ELEC HYD PUMPS LEFT 15
		"laminar/B738/toggle_switch/electric_hydro_pumps2_pos",		//ELEC HYD PUMPS RIGHT 16
		"laminar/B738/air/trim_air_pos",							//TRIM AIR 17
		"laminar/B738/toggle_switch/bleed_air_1_pos",				//ENGINE BLEED 1 18
		"laminar/B738/toggle_switch/bleed_air_2_pos",				//ENGINE BLEED 2 19
		"laminar/B738/toggle_switch/bleed_air_apu_pos",				//APU BLEED 20
		"sim/cockpit/electrical/beacon_lights_on",					//ANTI COLLISION LIGHTS 21
		"laminar/B738/autobrake/autobrake_RTO_arm",					//RTO arm 22
		"laminar/B738/toggle_switch/capt_probes_pos",				//PROBE HEAT L 23
		"laminar/B738/toggle_switch/fo_probes_pos",					//PROBE HEAT R 24
		""
	};

	int ProcedureType = 0;
	int ProcedureStage = 0;
	float timeElapsed = 0;
	bool doneProcedures = false;
	bool powerUpProcedures = false;
	bool preflightProcedures = false;
	bool beforeTaxiProcedures = false;
	bool beforeTakeoffProcedures = false;
	bool cleanUpProcedures = false;
	bool shutdownProcedures = false;

	// printname is not defined inside class defination
	void doProcedures(float elapsed);
};
#endif