#pragma once
#ifndef ShutdownProcedure_H
#define ShutdownProcedure_H

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

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

using namespace std;

class ShutdownProcedure
{
public:

	static int shutdownProcedure(int stage);

};
#endif