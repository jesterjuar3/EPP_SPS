/*****************************************************************************
*                                                                            *
*       Source title:   TemplateProgram_ETS88TH_TestFunction.CPP
*                       (xxx SHORT DESCRIPTION xxx)
*         Written by:   nnnnn n. nnnnn                                       *
*   Last Modified by:                                                        *
*               Date:   mm/dd/yy                                             *
*        Current Rev:   1.01                                                 *
*                                                                            *
*        Description:                                                        *
*                                                                            *
*   Revision History:                                                        *
*                                                                            *
*     mm/dd/yy  r.rr  - Original coding.                                     *
*     08/08/17  r.rr  - Original coding.                                     *
*                                                                            *
*****************************************************************************/
#include "../ONSEMI_EPP_SPS_WS.h"
#include "../expansion_bits.h"

/// \brief Local Userinit
int	Userinit_VTH_SPU(LPCTSTR TestLabel)
{
	int ret = SUCCESS;
	// place here code to execute once, e.g.
	// ret |= apu32patternloader(...);
	return ret;
}

///<  \brief Test Function: VTH_SPU
ETS_PRGFLOW_FUNC VTH_SPU(int DSIndex, LPCTSTR TestLabel)
{
#pragma region UserInit
	if (DSIndex == USERINIT)
	{
		int ret = SUCCESS;
		// In UserInit() you may call VTH_SPU(USERINIT,"") to get one time initialization
		if (clMULTI_JOBS_UTIL::IsTestFunctionEnabled(TestLabel))
			ret |= Userinit_VTH_SPU(TestLabel);
		return ret;
		// nothing else gets executed here
	}
	else if (DSIndex < 0)
	{ // Error checking
		std::string msg = "Unexpected DSIndex =" + std::to_string((long long)DSIndex) + " at " + std::string(TestLabel);
		etsfatalerror((char*)msg.c_str());
		return -1; // Fail as error if improperly called
	}
#pragma endregion

	Datalog.SetDSIndex(&DSIndex);
	Datalog.LogTestLabel(TestLabel);
	CResults VTH_SPUResult;

#pragma region Setup
	/*
		if (! isAlreadySetup()) {
			BasicSetup(); // ideally fall here only if this is the first executed TestFunction
			// just apply delta from Basic
		} else {
			// just apply delta from previous
		}
	*/
#pragma endregion // Setup

#pragma region TestCode

	double I = pIGE[DSIndex] * pds_scale(pIGEUnit[DSIndex], A);
	double HiLimit = HiLim[DSIndex] * pds_scale(Units[DSIndex], V);
	int spVRng = sp112BestVRange(HiLimit); //Determine VRange based on VTH HiLimit
	int spIRng = sp112BestIRange(spVRng, I); //Determine IRange based on VRange and FI value
	if ((spIRng == INVALID) || (spVRng == INVALID))
		return FAIL;

	//=========================================================================================================
	//Setup MUX Board
	xbit.close(KXB16);
	lwait(3 MSEC);
	//=========================================================================================================

	//CBit.Close("VTH_SPU", MS_ALL);
	if (QuadSite) {
		CBit.Close("VTH_SPU");
	}
	else {
		CBit.Close("VTH_SPU_HC");
	}
	Timer.WaitWait();

	//Setup instruments for the tests

	sp112set(SPU112, SP_FI, -I, spVRng, spIRng);
	lwait(pTSettle[DSIndex]);

	sp112mv(SPU112, SP_MV_1X, SP_MI_1X, pTSample[DSIndex], 13);

	VTH_SPUResult.GetResults(-1, MS_ALL);
	//Discharge & Disconnect

	sp112set(SPU112, SP_FV, 0, spVRng, spIRng);
	lwait(1 MSEC);
	sp112set(SPU112, SP_OFF, 0, spVRng, spIRng);

	if (QuadSite) {
		CBit.Open("VTH_SPU");
	}
	else {
		CBit.Open("VTH_SPU_HC");
	}
	Timer.WaitWait();

	//=========================================================================================================
	//Open MUX Board
	xbit.open(KXB16);
	lwait(3 MSEC);
	//=========================================================================================================

	Datalog_TestLimits(VTH_SPUResult, DSIndex, TestLabel);
#pragma endregion // TestCode
	// Reset tester setup to properly match next test block	
	return(msSiteStat(MS_ALL)); // Return w/status
} // END_ETS_PRGFLOW_FUNC
