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

#include "DataRefList.h"
#include "ClimbProcedures.h"

int ClimbProcedures::gearUp(int stage)
{

	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/push_button/gear_up"));
		return 2;
	}
	return 0;
}
int ClimbProcedures::flaps1(int stage)
{

	if (stage == 0) {
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.125);
		return 2;
	}
	return 0;
}
int ClimbProcedures::flaps0(int stage)
{

	if (stage == 0) {
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.0);
		return 2;
	}
	return 0;
}
int ClimbProcedures::afterTakeoff(int stage)
{

	if (stage == 0) {
		XPLMSetDatai(XPLMFindDataRef(DataRefList::dataRefList[27]), 1);
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/push_button/gear_off"));
		return 2;
	}
	return 0;
}
int ClimbProcedures::tenk(int stage)
{
	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng1_start_left"));
		return 1;
	}
	if (stage == 1) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng2_start_left"));
		return 1;
	}
	if (stage == 2) {
		XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_off"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_left_off"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_right_off"));
		XPLMSpeakString("Ten K Climb Procedures Completed");
		return 2;
	}
	return 0;
}
