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
#include <string.h>
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
#include <stdlib.h>

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

class SubHandler
{
public:
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