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
#include "DescentProcedures.h"

int DescentProcedures::tenk(int stage)
{

	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng1_start_right"));
		return 1;
	}
	if (stage == 1) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/knob/eng2_start_right"));
		return 1;
	}
	if (stage == 2) {
		XPLMCommandOnce(XPLMFindCommand("sim/lights/landing_lights_on"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_left_on"));
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/switch/rwy_light_right_on"));
		XPLMSpeakString("Ten K Descent Procedures Completed");
		return 2;
	}
	return 0;
}
int DescentProcedures::flaps1(int stage)
{

	if (stage == 0) {
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.125);
		return 2;
	}
	return 0;
}
int DescentProcedures::flaps5(int stage)
{

	if (stage == 0) {
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.375);
		return 2;
	}
	return 0;
}
int DescentProcedures::flaps15(int stage)
{

	if (stage == 0) {
		XPLMCommandOnce(XPLMFindCommand("laminar/B738/push_button/gear_down"));
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.625);
		return 2;
	}
	return 0;
}
int DescentProcedures::flaps30(int stage)
{

	if (stage == 0) {
		XPLMSetDataf(XPLMFindDataRef(DataRefList::dataRefList[26]), 0.875);
		return 2;
	}
	return 0;
}