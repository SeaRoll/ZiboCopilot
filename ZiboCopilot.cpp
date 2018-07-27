//Created by Yohan Joo 18-06-21
/*
	IF YOU READ THIS. YOUR EYES WILL BLEED
	THIS IS HORRIBLE PROGRAMMING PRACTICE AND I KNOW IT.
	I SUCK AT PROGRAMMING HELP.

*/

#if APL
#if defined(__MACH__)
#include <Carbon/Carbon.h>
#endif
#endif
#include "XPLMMenus.h"
#include <cstdio>
#include <cstring>
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

#include "SubHandler.h"
SubHandler subHandler;
XPLMMenuID g_menu_id;
int g_menu_container_idx;
int current_procedure = 0;


//Commands
XPLMCommandRef cmdpowerUpProcedures = nullptr;
XPLMCommandRef cmdpreflightProcedures = nullptr;
XPLMCommandRef cmdbeforeTaxiProcedures = nullptr;
XPLMCommandRef cmdbeforeTakeoffProcedures = nullptr;
XPLMCommandRef cmdcleanUpProcedures = nullptr;
XPLMCommandRef cmdshutdownProcedures = nullptr;
XPLMCommandRef cmdnextStep = nullptr;


//-------------------------------------------------------- Initiate Functions ------------------------------------//
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);
void startFunction(SubHandler::Procedures procedure);
void doNextProcedure();
int funcpowerUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcpreflightProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcbeforeTaxiProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcbeforeTakeoffProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funccleanUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcshutdownProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcnextProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
void menu_handler(void *, void *);


//-------------------------------------------------------- START OF PLUGIN --------------------------------------//
PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc)
{
	strcpy(outName, "ZiboCopilot");
	strcpy(outSig, "yohan.copilot.zibocopilot");
	strcpy(outDesc, "Copilot (like FS2Crew) for Zibo 737");

	g_menu_container_idx = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "Copilot Menu", 0, 0);
	g_menu_id = XPLMCreateMenu("Copilot Menu", XPLMFindPluginsMenu(), g_menu_container_idx, menu_handler, nullptr);
	XPLMAppendMenuItem(g_menu_id, "Powerup Procedure", static_cast<void *>("power_up"), 1);
	XPLMAppendMenuItem(g_menu_id, "Preflight Procedures", static_cast<void *>("preflight"), 1);
	XPLMAppendMenuItem(g_menu_id, "Before Taxi Procedures", static_cast<void *>("before_taxi"), 1);
	XPLMAppendMenuItem(g_menu_id, "Before Takeoff Procedures", static_cast<void *>("before_takeoff"), 1);
	XPLMAppendMenuItem(g_menu_id, "Clean Up Procedures", static_cast<void *>("clean_up"), 1);
	XPLMAppendMenuItem(g_menu_id, "Shutdown Procedures", static_cast<void *>("shutdown"), 1);
	XPLMAppendMenuSeparator(g_menu_id);
	XPLMAppendMenuItem(g_menu_id, "Next Procedure", static_cast<void *>("next_procedure"), 1);

	const auto aircraft_menu = XPLMFindAircraftMenu();
	if (aircraft_menu) // This will be nullptr unless this plugin was loaded with an aircraft (i.e., it was located in the current aircraft's "plugins" subdirectory)
	{
		XPLMAppendMenuItemWithCommand(aircraft_menu, "Toggle Settings (Command-Based)", XPLMFindCommand("sim/operation/toggle_settings_window"));
	}

	//Commands
	cmdpowerUpProcedures = XPLMCreateCommand("737/Zibocopilot/powerUpProcedure", "Power Up Procedure");
	cmdpreflightProcedures = XPLMCreateCommand("737/Zibocopilot/preflightProcedure", "Preflight Procedure");
	cmdbeforeTaxiProcedures = XPLMCreateCommand("737/Zibocopilot/beforeTaxiProcedure", "Before Taxi Procedure");
	cmdbeforeTakeoffProcedures = XPLMCreateCommand("737/Zibocopilot/beforeTakeoffProcedure", "Before Takeoff Procedure");
	cmdcleanUpProcedures = XPLMCreateCommand("737/Zibocopilot/cleanUpProcedure", "Clean Up Procedure");
	cmdshutdownProcedures = XPLMCreateCommand("737/Zibocopilot/shutdownProcedure", "Shutdown Procedure");
	cmdnextStep = XPLMCreateCommand("737/Zibocopilot/nextStep", "Next Procedure");
	XPLMRegisterCommandHandler(cmdpowerUpProcedures, funcpowerUpProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdpreflightProcedures, funcpreflightProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdbeforeTaxiProcedures, funcbeforeTaxiProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdbeforeTakeoffProcedures, funcbeforeTakeoffProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdcleanUpProcedures, funccleanUpProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdshutdownProcedures, funcshutdownProcedures, 1, nullptr);
	XPLMRegisterCommandHandler(cmdnextStep, funcnextProcedures, 1, nullptr);

	return 1;
}


//-------------------------------------------------------- DO NOT TOUCH -----------------------------------------//
PLUGIN_API void XPluginStop()
{
	// Since we created this menu, we'll be good citizens and clean it up as well
	XPLMDestroyMenu(g_menu_id);
	// If we were able to add a command to the aircraft menu, it will be automatically removed for us when we're unloaded
}

PLUGIN_API void XPluginDisable() { }
PLUGIN_API int XPluginEnable() { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }
//-------------------------------------------------------- END --------------------------------------------------//


//-------------------------------------------------------- SUB MENU HANDLER -------------------------------------//
void menu_handler(void * in_menu_ref, void * in_item_ref)
{
	const auto compare_string = static_cast<const char *>(in_item_ref);
	if (!strcmp(compare_string, "power_up"))
	{
		startFunction(SubHandler::powerUp);
	}
	else if (!strcmp(compare_string, "preflight"))
	{
		startFunction(SubHandler::preFlight);
	}
	else if (!strcmp(compare_string, "before_taxi"))
	{
		startFunction(SubHandler::beforeTaxi);
	}
	else if (!strcmp(compare_string, "before_takeoff"))
	{
		startFunction(SubHandler::beforeTakeOff);
	}
	else if (!strcmp(compare_string, "clean_up"))
	{
		startFunction(SubHandler::cleanUp);
	}
	else if (!strcmp(compare_string, "shutdown"))
	{
		startFunction(SubHandler::shutdown);
	}
	else if (!strcmp(compare_string, "next_procedure"))
	{
		doNextProcedure();
	}
}


//-------------------------------------------------------- Procedure Starter ------------------------------------//
void startFunction (SubHandler::Procedures procedure) {
	if (procedure == SubHandler::powerUp)
	{
		if (!subHandler.procedures[SubHandler::Procedures::powerUp]) {
			XPLMSpeakString("Beginning Powerup Procedures");
			subHandler.procedures[SubHandler::Procedures::powerUp] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::powerUp] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
	else if (procedure == SubHandler::preFlight)
	{
		if (!subHandler.procedures[SubHandler::Procedures::preFlight]) {
			XPLMSpeakString("Beginning Preflight Procedures");
			subHandler.procedures[SubHandler::Procedures::preFlight] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::preFlight] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
	else if (procedure == SubHandler::beforeTaxi)
	{
		if (!subHandler.procedures[SubHandler::Procedures::beforeTaxi]) {
			XPLMSpeakString("Beginning Before Taxi Procedures");
			subHandler.procedures[SubHandler::Procedures::beforeTaxi] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::beforeTaxi] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
	else if (procedure == SubHandler::beforeTakeOff)
	{
		if (!subHandler.procedures[SubHandler::Procedures::beforeTakeOff]) {
			XPLMSpeakString("Beginning Before Takeoff Procedures");
			subHandler.procedures[SubHandler::Procedures::beforeTakeOff] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::beforeTakeOff] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
	else if (procedure == SubHandler::cleanUp)
	{
		if (!subHandler.procedures[SubHandler::Procedures::cleanUp]) {
			XPLMSpeakString("Beginning Clean Up Procedures");
			subHandler.procedures[SubHandler::Procedures::cleanUp] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::cleanUp] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
	else if (procedure == SubHandler::shutdown)
	{
		if (!subHandler.procedures[SubHandler::Procedures::shutdown]) {
			XPLMSpeakString("Beginning Shutdown Procedures");
			subHandler.procedures[SubHandler::Procedures::shutdown] = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, nullptr);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.procedures[SubHandler::Procedures::shutdown] = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		}
	}
}


//-------------------------------------------------------- Procedure Loop ---------------------------------------//
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon)
{
	/* Elapsed Time */
	const auto elapsed = XPLMGetElapsedTime();

	if (!subHandler.doneProcedures) {
		subHandler.doProcedures(elapsed);
	}
	else {
		XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, nullptr);
		subHandler.doneProcedures = false;
	}

	/* Return 1.0 to indicate that we want to be called again in 1 second. */
	return 1.0;
}


//-------------------------------------------------------- Command Functions ------------------------------------//
int funcpowerUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) { 
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::powerUp);
	}
	return 0;
}
int funcpreflightProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::preFlight);
	}
	return 0;
}
int funcbeforeTaxiProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::beforeTaxi);
	}
	return 0;
}
int funcbeforeTakeoffProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::beforeTakeOff);
	}
	return 0;
}
int funccleanUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::cleanUp);
	}
	return 0;
}
int funcshutdownProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(SubHandler::shutdown);
	}
	return 0;
}
int funcnextProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		doNextProcedure();
	}
	return 0;
}


//-------------------------------------------------------- Next Procedure Function ------------------------------//
void doNextProcedure()
{
	if (subHandler.ProcedureType < SubHandler::Procedures::COUNT)
		startFunction(static_cast<SubHandler::Procedures>(subHandler.ProcedureType));
}
