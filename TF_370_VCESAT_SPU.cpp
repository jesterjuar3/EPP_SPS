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
int	Userinit_VCESAT_SPU(LPCTSTR TestLabel)
{
	int ret = SUCCESS;
	// place here code to execute once, e.g.
	// ret |= apu32patternloader(...);
	return ret;
}

///<  \brief Test Function: VCESAT_SPU
ETS_PRGFLOW_FUNC VCESAT_SPU(int DSIndex, LPCTSTR TestLabel)
{
	int startDSIdx = DSIndex; // This is used to reference the cRamCntrl Map!! Do not change this!!!
	static bool doSerial = true;
	static std::map< int, vector <cRamCtrl> > ramCtrlMap; // Defined and created only once since a static variable. Exsits for the entirety of program run.

#pragma region UserInit
	if (DSIndex == USERINIT)
	{
		int ret = SUCCESS;
		ramCtrlMap.clear();
		create_and_add_cRamCtrl("VCESAT_SPU", doSerial, ramCtrlMap);
		// In UserInit() you may call VCESAT_SPU(USERINIT,"") to get one time initialization
		if (clMULTI_JOBS_UTIL::IsTestFunctionEnabled(TestLabel))
			ret |= Userinit_VCESAT_SPU(TestLabel);
		return ret;
	}
	else if (DSIndex < 0)
	{ // Error checking
		std::string msg = "Unexpected DSIndex =" + std::to_string((long long)DSIndex) + " at " + std::string(TestLabel);
		etsfatalerror((char*)msg.c_str());
		return -1; // Fail as error if improperly called
	}

	if (!ramCtrlMap.count(startDSIdx)) //count returns 0 if exists, or 1 if it does not. 
	{
		etsfatalerror("VCESat(): No cRamCtrl object exsits for this DSIndex!! Please see the UserInit section of VCESat to make sure the object exists.");
		etsfatalerror("VCESat(): Exiting fucntion without executing.");
		return (msSiteStat(MS_ALL)); // Exit the function early since the cRamCtrl object does not exist!
	}

#pragma endregion

	Datalog.SetDSIndex(&DSIndex);
	Datalog.LogTestLabel(TestLabel);

#pragma region Setup

#pragma endregion // Setup

#pragma region TestCode

	auto& VCESatTest = ramCtrlMap[startDSIdx];
	double VGE = pVGE[DSIndex];
	int mux = pMux[DSIndex];
	bool Inst = true; // false = APU, true = SPU
	size_t numGvals = 0;
	int site;

	// Set the number of different gate values
	// For each different gate value in the test, there is a separate object.
	if (QuadSite && doSerial)
	{
		// If testing quad site and full serial, there will be one object per site
		// and one object per gate value. 
		// So if there were only 1 gate value, there would be 4 objects (1 per site)
		// and if there were 2 gate values, there would be 8 objects total ( 2 per site)
		// so divide by # of sites here to "reverse engineer" the number of gate values
		numGvals = VCESatTest.size() / 4;
	}
	else numGvals = VCESatTest.size(); // testing single site or Ram-style, no need to divide by # of sites

	//Results class
	// Get the # of pulses to be datalogged from the cRamCtrl object (which gets its info from the PDS)
	// And then create the CResults based on the # of pulses
	CResults VCESATresults(Get_Total_Pulses(doSerial, VCESatTest));

	int num_loops = 1;   // Single site (HC DUT) or RAM-style Quadsite (LC DUT)
	if (QuadSite && doSerial)  num_loops = 4; // Full Serial style 
	int charged = 1;

	//=========================================================================================================
	//Setup MUX Board
	switch (mux) {
	case 1:
		xbit.close(KXB01, KXB16); // VIN_A, AGND_A
		break;
	case 2:
		xbit.close(KXB02, KXB16); // SW_A, AGND_A
		break;
	case 3:
		xbit.close(KXB03, KXB16); // PH_A, AGND_A
		break;
	case 4:
		xbit.close(KXB25, KXB16); // HS_SRC_A, AGND_A
		break;
	case 6:
		xbit.close(KXB08, KXB16); // BST_A, AGND_A
		break;
	case 7:
		xbit.close(KXB13, KXB16); // VDD_A, AGND_A
		break;
	case 8:
		xbit.close(KXB10, KXB16); // IOUT_A, AGND_A
		break;
	case 9:
		xbit.close(KXB12, KXB16); // TOUT_A, AGND_A
		break;
	case 11:
		xbit.close(KXB06, KXB16); // PWM_A, AGND_A
		break;
	case 12:
		xbit.close(KXB34, KXB36); // VIN_B, AGND_B
		break;
	case 13:
		xbit.close(KXB03, KXB36); // SW_B, AGND_B
		break;
	case 14:
		xbit.close(KXB05, KXB36); // PH_B, AGND_B
		break;
	case 15:
		xbit.close(KXB26, KXB36); // HS_SRC_B, AGND_B
		break;
	case 17:
		xbit.close(KXB09, KXB36); // BST_B, AGND_B
		break;
	case 18:
		xbit.close(KXB15, KXB36); // VDD_B, AGND_B
		break;
	case 19:
		xbit.close(KXB11, KXB36); // IOUT_B, AGND_B
		break;
	case 20:
		xbit.close(KXB35, KXB36); // TOUT_B, AGND_B
		break;
	case 22:
		xbit.close(KXB07, KXB36); // PWM_B, AGND_B
		break;
	case 23:
		xbit.close(KXB08, KXB21); // BST_A, VDD_A
		break;
	case 24:
		xbit.close(KXB09, KXB22); // BST_B, VDD_B
		break;
	case 25:
		xbit.close(KXB19, KXB13, KXB16); // EN_A, VDD_A, AGND_A
		break;
	case 26:
		xbit.close(KXB19, KXB13, KXB16); // EN_A, VDD_A, AGND_A
		break;
	case 27:
		xbit.close(KXB20, KXB14, KXB36); // EN_B, VDD_B, AGND_B
		break;
	case 28:
		xbit.close(KXB20, KXB14, KXB36); // EN_B, VDD_B, AGND_B
		break;
	case 29:
		xbit.close(KXB01, KXB27); // VIN_A, HS_SRC_A
		break;
	case 30:
		xbit.close(KXB25, KXB31); // HS_SRC_A, PGND_A
		break;
	case 31:
		xbit.close(KXB34, KXB28); // VIN_B, HS_SRC_B
		break;
	case 32:
		xbit.close(KXB26, KXB32); // HS_SRC_B, PGND_B
		break;
	default:
		// Handle invalid mux
		break;
	}
	lwait(3 MSEC);
	//=========================================================================================================

	// These functions are multi-site , 
	// so we only need to use one of the cRamCtrl objects to set up SPU for these tests. 

	FOR_EACH_SITE(site, NUM_SITES) {
		//gduvset(GDU[site], GDU830_CHARGE, VGE, GDU830_WAIT, site);
		if (VGE > 0)
			gdu830vset(GDU[site], GDU830_CHARGE, 0.0, VGE, GDU830_NO_WAIT, site);
		else
			gdu830vset(GDU[site], GDU830_CHARGE, VGE, 0.0, GDU830_NO_WAIT, site);

	}

	VCESatTest.at(0).Setup_Measure_Instrument(Inst);

	// This variable is used at the end to make sure we can collect all pulse data into the GlobalResults array for use with delvsat test functions
	int totalSubtests = 0;

	FOR_EACH_SITE(site, NUM_SITES) {

		int offsetDifGVals = 0;

		if (QuadSite) {
			CBit.Close("VCESat_SPU", site);
		}
		else {
			CBit.Close("VCESat_SPU_HC", site);
		}

		Timer.WaitWait();

		if (QuadSite && doSerial) { offsetDifGVals = 4; }// this number skips the 4 sites data
		else { offsetDifGVals = 0; }

		// this keeps track of the total number of pulses in the test so that it can be datalogged correctly
		int totPCount = 0;

		// Now we go into a for-loop that is based on the number of different gate values!
		// Use numGvals-1, in order to keep 0-based looping
		// Example: if numGvales = 2, we'll go through the loop i=0, and i =1 (2 times).
		gdu830completecharge(GDU[site], site);
		for (auto i = 0; i <= (numGvals - 1); i++)
		{
			// This is the index we use.
			// s is the site number, i is for the gate value iterator
			auto index = site + (i * offsetDifGVals);
			if (!QuadSite) index = 0;
			VCESatTest.at(index).Setup_Everything(Inst); // setup the resources

			// Run the test!
			mcurun(VCESatTest.at(index).GetSeqName(), NULL, 1); // Run the sequence associated with the HPU, GDU, and SPU112 ADC mode

			// Collect the data!
			if (gbOnLine) VCESatTest.at(index).Capture_All_Waveforms(); // this will cause bombs offline

			// "Cherry-pick" the data from the SPUs and put into a 2-d vector, to be datalogged later
			if (gbOnLine)
			{
				// If testing quad site full serial or Single HC Site, only need to move data for one site
				if ((QuadSite && doSerial) || !QuadSite)
					VCESatTest.at(index).Process_Waveforms_For_Datalog(site, Inst);
				// If testing RAM-style, need to move data for all sites into datalog.
				else
					VCESatTest.at(index).Process_Waveforms_For_Datalog_All_Sites(Inst);
			}
			// Transfer results to results object to be datalogged!
			// Weave together the datalog for the different gate values:
			Save_Total_Results(site, VCESatTest.at(index), VCESATresults, totPCount);
			totPCount += VCESatTest.at(index).GetPulseNum();
		}

		// Must turn off the GDU before going to the next site, or else will get a
		// "Pattern was not selected" error with mcurun().
		// Do not remove this for TTR! 
		if (QuadSite && doSerial) gdu830vset(GDU[site], GDU830_CHARGE, 0.0, VGE, GDU830_NO_WAIT, site);

		VCESatTest.at(0).HPU_Standby(); // Avoid HPU to be forcing 0 when switching sites

		if (QuadSite) {
			CBit.Open("VCESat_SPU", site);
		}
		else {
			CBit.Open("VCESat_SPU_HC", site);
		}
		totalSubtests = totPCount;

	}

	VCESatTest.at(0).Shutdown_Resources(Inst);

	//=========================================================================================================
	//Open MUX Board
	switch (mux) {
	case 1:
		xbit.open(KXB01, KXB16); // VIN_A, AGND_A
		break;
	case 2:
		xbit.open(KXB02, KXB16); // SW_A, AGND_A
		break;
	case 3:
		xbit.open(KXB03, KXB16); // PH_A, AGND_A
		break;
	case 4:
		xbit.open(KXB25, KXB16); // HS_SRC_A, AGND_A
		break;
	case 6:
		xbit.open(KXB08, KXB16); // BST_A, AGND_A
		break;
	case 7:
		xbit.open(KXB13, KXB16); // VDD_A, AGND_A
		break;
	case 8:
		xbit.open(KXB10, KXB16); // IOUT_A, AGND_A
		break;
	case 9:
		xbit.open(KXB12, KXB16); // TOUT_A, AGND_A
		break;
	case 11:
		xbit.open(KXB06, KXB16); // PWM_A, AGND_A
		break;
	case 12:
		xbit.open(KXB34, KXB36); // VIN_B, AGND_B
		break;
	case 13:
		xbit.open(KXB03, KXB36); // SW_B, AGND_B
		break;
	case 14:
		xbit.open(KXB05, KXB36); // PH_B, AGND_B
		break;
	case 15:
		xbit.open(KXB26, KXB36); // HS_SRC_B, AGND_B
		break;
	case 17:
		xbit.open(KXB09, KXB36); // BST_B, AGND_B
		break;
	case 18:
		xbit.open(KXB15, KXB36); // VDD_B, AGND_B
		break;
	case 19:
		xbit.open(KXB11, KXB36); // IOUT_B, AGND_B
		break;
	case 20:
		xbit.open(KXB35, KXB36); // TOUT_B, AGND_B
		break;
	case 22:
		xbit.open(KXB07, KXB36); // PWM_B, AGND_B
		break;
	case 23:
		xbit.open(KXB08, KXB21); // BST_A, VDD_A
		break;
	case 24:
		xbit.open(KXB09, KXB22); // BST_B, VDD_B
		break;
	case 25:
		xbit.open(KXB19, KXB13, KXB16); // EN_A, VDD_A, AGND_A
		break;
	case 26:
		xbit.open(KXB19, KXB13, KXB16); // EN_A, VDD_A, AGND_A
		break;
	case 27:
		xbit.open(KXB20, KXB14, KXB36); // EN_B, VDD_B, AGND_B
		break;
	case 28:
		xbit.open(KXB20, KXB14, KXB36); // EN_B, VDD_B, AGND_B
		break;
	case 29:
		xbit.open(KXB01, KXB27); // VIN_A, HS_SRC_A
		break;
	case 30:
		xbit.open(KXB25, KXB31); // HS_SRC_A, PGND_A
		break;
	case 31:
		xbit.open(KXB34, KXB28); // VIN_B, HS_SRC_B
		break;
	case 32:
		xbit.open(KXB26, KXB32); // HS_SRC_B, PGND_B
		break;
	default:
		// Handle invalid mux
		break;
	}
	lwait(3 MSEC);
	//=========================================================================================================

	Datalog_TestLimits(VCESATresults, DSIndex, TestLabel);
#pragma endregion // TestCode

	// AVOID ALARMS BEING KEPT OFF UNINTENTIONALLY
	if (1) SetAllAlarmsOn();
	alarmmask(ALL_HPU, ALARM_DISABLE, ALARM_DUT_INTERLOCK_NOT_SATISFIED, ALARM_NOACTION, ALARM_NOACTION, MS_ALL);

	return(msSiteStat(MS_ALL)); // Return w/status
} // END_ETS_PRGFLOW_FUNC
