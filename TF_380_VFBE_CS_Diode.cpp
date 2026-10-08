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

extern void getBV_dsidx(std::vector <int>& vecRef);
extern bool checkIfBV(int dsIdx, std::vector <int>& vecRef);

/// \brief Local Userinit
int	Userinit_VFBE_CS_Diode( LPCTSTR TestLabel )
{
	int ret=SUCCESS;
	// place here code to execute once, e.g.
	// ret |= apu32patternloader(...);
	return ret;
}

///<  \brief Test Function: VFBE_CS_Diode
ETS_PRGFLOW_FUNC VFBE_CS_Diode( int DSIndex, LPCTSTR TestLabel )
{
	static std::vector<int> BVdsidxes; // Defined and created only once since a static variable.
    #pragma region UserInit
	if (DSIndex == USERINIT)
	{
		int ret=SUCCESS;
		// In UserInit() you may call VFBE_CS_Diode(USERINIT,"") to get one time initialization
		getBV_dsidx(BVdsidxes);
		if (clMULTI_JOBS_UTIL::IsTestFunctionEnabled(TestLabel)) 
			ret |= Userinit_VFBE_CS_Diode(TestLabel);
		return ret;
		// nothing else gets executed here
	}
	else if (DSIndex < 0) 
	{ // Error checking
		std::string msg="Unexpected DSIndex =" + std::to_string((long long)DSIndex) + " at "  + std::string(TestLabel);
		etsfatalerror((char *)msg.c_str());
		return -1; // Fail as error if improperly called
	}
#pragma endregion

	Datalog.SetDSIndex( &DSIndex );
	Datalog.LogTestLabel( TestLabel );

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
	// Write here the actual test implementation and data logs
	CResults VFBEResults(NUM_SITES);
	double I = pICE[DSIndex] * pds_scale(pICEUnit[DSIndex], A);			//Obtaining forcing parameters from the PDS
	double HiLimit = HiLim[DSIndex] * pds_scale(Units[DSIndex], V);
	int spVRng = sp112BestVRange(HiLimit);								//Determine VRange based on VTH HiLimit
	int spIRng = sp112BestIRange(spVRng, I);							//Determine IRange based on VRange and FI value
	int site;
	int mux = pDCpw[DSIndex];
	
	switch (mux) {
		case 1:  xbit.close(KXB06, KXB11); break;
		case 2:  xbit.close(KXB23, KXB11); break;
		case 3:  xbit.close(KXB40, KXB11); break;
		case 4:  xbit.close(KXB01, KXB11); break;

		case 6:  xbit.close(KXB12, KXB11); break;
		case 7:  xbit.close(KXB15, KXB11); break;
		case 8:  xbit.close(KXB08, KXB11); break;
		case 9:  xbit.close(KXB14, KXB11); break;

		case 11: xbit.close(KXB13, KXB11); break;
		case 12: xbit.close(KXB29, KXB35); break;
		case 13: xbit.close(KXB22, KXB35); break;
		case 14: xbit.close(KXB26, KXB35); break;
		case 15: xbit.close(KXB30, KXB35); break;

		case 17: xbit.close(KXB28, KXB35); break;
		case 18: xbit.close(KXB37, KXB35); break;
		case 19: xbit.close(KXB34, KXB35); break;
		case 20: xbit.close(KXB32, KXB35); break;

		case 22: xbit.close(KXB33, KXB35); break;
		case 23: xbit.close(KXB12, KXB09); break;
		case 24: xbit.close(KXB28, KXB38); break;

		case 29: xbit.close(KXB06, KXB05); break;
		case 30: xbit.close(KXB01, KXB04); break;
		case 31: xbit.close(KXB29, KXB31); break;
		case 32: xbit.close(KXB30, KXB20); break;

		default:
			etsprintf("Invalid Mux")
	}

	if ((spIRng == INVALID) || (spVRng == INVALID))	return FAIL;
	//Hardware Setup for testing
	CBit.Close("VFBE_CS_Diode, K62_DIB", MS_ALL);
	Timer.WaitWait();


	sp112set(SPU112, SP_FI, I, spVRng, spIRng);	
	//Apply ICE to Temperature Diode
	//lwait(pTSettle[DSIndex]);
	lwait(10 MSEC);
	sp112mv(SPU112, SP_MV_1X, SP_MI_1X, pTSample[DSIndex], 13.0);		//Reading voltage from the Temperature Diode
	//sp112mv(SPU112, SP_MV_1X, SP_MI_1X, 4000, 13.0);
	VFBEResults.GetResults(-1);

	sp112set(SPU112, SP_FI, 0.0, spVRng, spIRng);
	sp112set(SPU112, SP_OFF, 0.0, spVRng, spIRng);

	switch (mux) {
		case 1:  xbit.open(KXB06, KXB11); break;
		case 2:  xbit.open(KXB23, KXB11); break;
		case 3:  xbit.open(KXB40, KXB11); break;
		case 4:  xbit.open(KXB01, KXB11); break;

		case 6:  xbit.open(KXB12, KXB11); break;
		case 7:  xbit.open(KXB15, KXB11); break;
		case 8:  xbit.open(KXB08, KXB11); break;
		case 9:  xbit.open(KXB14, KXB11); break;

		case 11: xbit.open(KXB13, KXB11); break;
		case 12: xbit.open(KXB29, KXB35); break;
		case 13: xbit.open(KXB22, KXB35); break;
		case 14: xbit.open(KXB26, KXB35); break;
		case 15: xbit.open(KXB30, KXB35); break;

		case 17: xbit.open(KXB28, KXB35); break;
		case 18: xbit.open(KXB37, KXB35); break;
		case 19: xbit.open(KXB34, KXB35); break;
		case 20: xbit.open(KXB32, KXB35); break;

		case 22: xbit.open(KXB33, KXB35); break;
		case 23: xbit.open(KXB12, KXB09); break;
		case 24: xbit.open(KXB28, KXB38); break;

		case 29: xbit.open(KXB06, KXB05); break;
		case 30: xbit.open(KXB01, KXB04); break;
		case 31: xbit.open(KXB29, KXB31); break;
		case 32: xbit.open(KXB30, KXB20); break;

		default:
			etsprintf("Invalid Mux")
	}

	Datalog.TestLimits(&VFBEResults);


	CBit.Open("VFBE_CS_Diode,K62_DIB", MS_ALL);
#pragma endregion // TestCode
	// Reset tester setup to properly match next test block
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
