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
#include "Procedures/PreflightProcedure.h"
#include "Procedures/BeforeTaxiProcedure.h"
#include "Procedures/BeforeTakeoffProcedure.h"
#include "Procedures/CleanupProcedure.h"
#include "Procedures/ShutdownProcedure.h"

void SubHandler::finishProcedure(int id)
{
	procedures[id] = false;
	ProcedureStage = 0;
	ProcedureType = id + 1;
	if (ProcedureType >= count)
		ProcedureType = 0;
	doneProcedures = true;
}


void SubHandler::doProcedures() {
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