//Created by Yohan Joo 18-06-21
/*
	IF YOU READ THIS. YOUR EYES WILL BLEED
	THIS IS HORRIBLE PROGRAMMING PRACTICE AND I KNOW IT.
	I SUCK AT PROGRAMMING HELP.


	ZIBO 3.27v changes:
	Cab UTIL and PASS is not required?
	Engine bleed is not required to put down.

*/

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

#include "SubHandler.h"
SubHandler subHandler;


//Commands
XPLMCommandRef cmdpowerUpProcedures = NULL;
XPLMCommandRef cmdpreflightProcedures = NULL;
XPLMCommandRef cmdbeforeTaxiProcedures = NULL;
XPLMCommandRef cmdbeforeTakeoffProcedures = NULL;
XPLMCommandRef cmdcleanUpProcedures = NULL;
XPLMCommandRef cmdshutdownProcedures = NULL;



static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);
void startFunction(int Phase);

//Command Functions
int funcpowerUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcpreflightProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcbeforeTaxiProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcbeforeTakeoffProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funccleanUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);
int funcshutdownProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);

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

	//Commands
	cmdpowerUpProcedures = XPLMCreateCommand("737/Zibocopilot/powerUpProcedure", "Power Up Procedure");
	cmdpreflightProcedures = XPLMCreateCommand("737/Zibocopilot/preflightProcedure", "Preflight Procedure");
	cmdbeforeTaxiProcedures = XPLMCreateCommand("737/Zibocopilot/beforeTaxiProcedure", "Before Taxi Procedure");
	cmdbeforeTakeoffProcedures = XPLMCreateCommand("737/Zibocopilot/beforeTakeoffProcedure", "Before Takeoff Procedure");
	cmdcleanUpProcedures = XPLMCreateCommand("737/Zibocopilot/cleanUpProcedure", "Clean Up Procedure");
	cmdshutdownProcedures = XPLMCreateCommand("737/Zibocopilot/shutdownProcedure", "Shutdown Procedure");
	XPLMRegisterCommandHandler(cmdpowerUpProcedures, funcpowerUpProcedures, 1, (void *)0);
	XPLMRegisterCommandHandler(cmdpreflightProcedures, funcpreflightProcedures, 1, (void *)0);
	XPLMRegisterCommandHandler(cmdbeforeTaxiProcedures, funcbeforeTaxiProcedures, 1, (void *)0);
	XPLMRegisterCommandHandler(cmdbeforeTakeoffProcedures, funcbeforeTakeoffProcedures, 1, (void *)0);
	XPLMRegisterCommandHandler(cmdcleanUpProcedures, funccleanUpProcedures, 1, (void *)0);
	XPLMRegisterCommandHandler(cmdshutdownProcedures, funcshutdownProcedures, 1, (void *)0);

	return 1;
}


//-------------------------------------------------------- DO NOT TOUCH ------------------------------------------//
PLUGIN_API void XPluginStop(void)
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
		startFunction(0);
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_2"))
	{
		startFunction(1);
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_3"))
	{
		startFunction(2);
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_4"))
	{
		startFunction(3);
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_clean"))
	{
		startFunction(4);
	}
	else if (!strcmp((const char *)in_item_ref, "FlightProd_shutdown"))
	{
		startFunction(5);
	}
}

void startFunction (int Phase) {
	if (Phase == 0)
	{
		if (!subHandler.powerUpProcedures) {
			XPLMSpeakString("Beginning Powerup Procedures");
			subHandler.powerUpProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.powerUpProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (Phase == 1)
	{
		if (!subHandler.preflightProcedures) {
			XPLMSpeakString("Beginning Preflight Procedures");
			subHandler.preflightProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.preflightProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (Phase == 2)
	{
		if (!subHandler.beforeTaxiProcedures) {
			XPLMSpeakString("Beginning Before Taxi Procedures");
			subHandler.beforeTaxiProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.beforeTaxiProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (Phase == 3)
	{
		if (!subHandler.beforeTakeoffProcedures) {
			XPLMSpeakString("Beginning Before Takeoff Procedures");
			subHandler.beforeTakeoffProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.beforeTakeoffProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (Phase == 4)
	{
		if (!subHandler.cleanUpProcedures) {
			XPLMSpeakString("Beginning Clean Up Procedures");
			subHandler.cleanUpProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.cleanUpProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
	else if (Phase == 5)
	{
		if (!subHandler.shutdownProcedures) {
			XPLMSpeakString("Beginning Shutdown Procedures");
			subHandler.shutdownProcedures = true;
			subHandler.timeElapsed = XPLMGetElapsedTime();
			XPLMRegisterFlightLoopCallback(MyFlightLoopCallback, 1.0, NULL);
		}
		else {
			subHandler.ProcedureStage = 0;
			subHandler.shutdownProcedures = false;
			XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		}
	}
}

//The loop for procedures
static float MyFlightLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon)
{
	/* Elapsed Time */
	float elapsed = XPLMGetElapsedTime();

	if (!subHandler.doneProcedures) {
		subHandler.doProcedures(elapsed);
	}
	else {
		XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
		subHandler.doneProcedures = false;
	}

	/* Return 1.0 to indicate that we want to be called again in 1 second. */
	return 1.0;
}

int funcpowerUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) { 
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(0);
	}
	return 0;
}
int funcpreflightProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(1);
	}
	return 0;
}
int funcbeforeTaxiProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(2);
	}
	return 0;
}
int funcbeforeTakeoffProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(3);
	}
	return 0;
}
int funccleanUpProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(4);
	}
	return 0;
}
int funcshutdownProcedures(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon) {
	if (inPhase == xplm_CommandBegin)
	{
		startFunction(5);
	}
	return 0;
}