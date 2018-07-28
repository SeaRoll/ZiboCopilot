#pragma once
#ifndef SubHandler_H
#define SubHandler_H

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

#include "DatarefList.h"

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

using namespace std;

class SubHandler : public DataRefList
{
public:
	enum Procedures
	{
		power_up,
		pre_flight,
		before_taxi,
		before_take_off,
		clean_up,
		shutdown,
		count
	};

	int ProcedureType = 0;
	int ProcedureStage = 0;
	float timeElapsed = 0;
	bool doneProcedures = false;
	bool procedures[count] = { false };

	// printname is not defined inside class defination
	void doProcedures(float elapsed);
	void finishProcedure(int id);
};
#endif