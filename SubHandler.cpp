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
#include "Procedures/PowerUpProcedure.h"
#include "Procedures/PowerUpProcedure.cpp"
#include "Procedures/PreflightProcedure.h"
#include "Procedures/PreflightProcedure.cpp"
#include "Procedures/BeforeTaxiProcedure.h"
#include "Procedures/BeforeTaxiProcedure.cpp"
#include "Procedures/BeforeTakeoffProcedure.h"
#include "Procedures/BeforeTakeoffProcedure.cpp"
#include "Procedures/CleanupProcedure.h"
#include "Procedures/CleanupProcedure.cpp"
#include "Procedures/ShutdownProcedure.h"
#include "Procedures/ShutdownProcedure.cpp"
#include "Procedures/ClimbProcedures.h"
#include "Procedures/ClimbProcedures.cpp"
#include "Procedures/DescentProcedures.h"
#include "Procedures/DescentProcedures.cpp"

void SubHandler::finishProcedure(int id)
{
	procedures[id] = false;
	ProcedureStage = 0;
	ProcedureType = id + 1;
	if (ProcedureType >= count)
		ProcedureType = 0;
	doneProcedures = true;
}


void SubHandler::doProcedures(float elapsed) {
	if (procedures[power_up]) {
		if(PowerUpProcedure::powerUpProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if(PowerUpProcedure::powerUpProcedure(ProcedureStage) == 2)
			finishProcedure(power_up);
	}

	if (procedures[pre_flight]) {
		if (PreflightProcedure::preflightProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if (PreflightProcedure::preflightProcedure(ProcedureStage) == 2)
			finishProcedure(pre_flight);

	}

	if (procedures[before_taxi]) {
		if (BeforeTaxiProcedure::beforeTaxiProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if (BeforeTaxiProcedure::beforeTaxiProcedure(ProcedureStage) == 2)
			finishProcedure(before_taxi);
	}

	if (procedures[before_take_off]) {
		if (BeforeTakeoffProcedure::beforeTakeoffProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if (BeforeTakeoffProcedure::beforeTakeoffProcedure(ProcedureStage) == 2)
			finishProcedure(before_take_off);
	}
	//Supplementary
	if (procedures[gear_up]) {
		if (ClimbProcedures::gearUp(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ClimbProcedures::gearUp(ProcedureStage) == 2)
			finishProcedure(gear_up);
	}
	if (procedures[climb_flaps1]) {
		if (ClimbProcedures::flaps1(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ClimbProcedures::flaps1(ProcedureStage) == 2)
			finishProcedure(climb_flaps1);
	}
	if (procedures[climb_flaps0]) {
		if (ClimbProcedures::flaps0(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ClimbProcedures::flaps0(ProcedureStage) == 2)
			finishProcedure(climb_flaps0);
	}
	if (procedures[after_takeoff]) {
		if (ClimbProcedures::afterTakeoff(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ClimbProcedures::afterTakeoff(ProcedureStage) == 2)
			finishProcedure(after_takeoff);
	}
	if (procedures[climb_tenk]) {
		if (ClimbProcedures::tenk(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ClimbProcedures::tenk(ProcedureStage) == 2)
			finishProcedure(climb_tenk);
	}
	//--des--//
	if (procedures[des_tenk]) {
		if (DescentProcedures::tenk(ProcedureStage) == 1)
			ProcedureStage++;
		else if (DescentProcedures::tenk(ProcedureStage) == 2)
			finishProcedure(des_tenk);
	}
	if (procedures[des_flaps1]) {
		if (DescentProcedures::flaps1(ProcedureStage) == 1)
			ProcedureStage++;
		else if (DescentProcedures::flaps1(ProcedureStage) == 2)
			finishProcedure(des_flaps1);
	}
	if (procedures[des_flaps5]) {
		if (DescentProcedures::flaps5(ProcedureStage) == 1)
			ProcedureStage++;
		else if (DescentProcedures::flaps5(ProcedureStage) == 2)
			finishProcedure(des_flaps5);
	}
	if (procedures[des_flaps15]) {
		if (DescentProcedures::flaps15(ProcedureStage) == 1)
			ProcedureStage++;
		else if (DescentProcedures::flaps15(ProcedureStage) == 2)
			finishProcedure(des_flaps15);
	}
	if (procedures[des_flaps30]) {
		if (DescentProcedures::flaps30(ProcedureStage) == 1)
			ProcedureStage++;
		else if (DescentProcedures::flaps30(ProcedureStage) == 2)
			finishProcedure(des_flaps30);
	}
	//------------------------------//
	if (procedures[clean_up]) {
		if (CleanupProcedure::cleanupProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if (CleanupProcedure::cleanupProcedure(ProcedureStage) == 2)
			finishProcedure(clean_up);
	}

	if (procedures[shutdown]) {
		if (ShutdownProcedure::shutdownProcedure(ProcedureStage) == 1)
			ProcedureStage++;
		else if (ShutdownProcedure::shutdownProcedure(ProcedureStage) == 2)
			finishProcedure(shutdown);
	}
}
