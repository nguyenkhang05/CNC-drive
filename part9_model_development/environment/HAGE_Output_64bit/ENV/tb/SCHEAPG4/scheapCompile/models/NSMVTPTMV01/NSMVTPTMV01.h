/*************************************************************************
 *
 * NSMVTPTMV01.h
 *
 * Copyright(c) 2017-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef __NSMVTPTMV01__
#define __NSMVTPTMV01__

#include "OSCI2.h"
#include "global.h"
#include "Common.h"
#include "G4_CommonPrototype.h"

#include "TPTM_def.h"

#include<map>

class SELF2DEST_BASE;
class TPTM;
class TPTMG;
class G4_CommonPrototype;
template< unsigned int BUSWIDTH > class OSCI2DCDR;


class NSMVTPTMV01 
: public G4_CommonPrototype
{
public:

//constructor destructor
	NSMVTPTMV01(sc_module_name name = "TPTM", const char* config_file = NULL); 
	~NSMVTPTMV01(void);
	

//varriable
	sc_in<bool> sys_resetn; // reset port
	sc_out<bool> **TPTM_IRQ; //output interrupt from interval timer
        sc_out<bool> ***INTTPTMU; //output interrupt from up counter timer
	sc_in<sc_dt::uint64> cpu_clk; //clock port

	TlmTargetSocket<BUS_WIDTH_VCI32> ts; // target socket 
	
	SELF2DEST_BASE *mpSELF_ADDRESS_DECODER; // pointer of TPTM_SELF2DEST_BASE instance 
	OSCI2DCDR<BUS_WIDTH_VCI32> *mpDECODER; // pointer of OSCI2DCDR instance 
        TPTM** mpTPTM; // pointer of TPTM instance
	TPTMG** mpTPTMG; // pointer of TPTMG instance

private :
	unsigned char mPeNum; // the number of PE
	char mMapFile[1024]; // map file name
	unsigned char *mpPeBmid; // argument is PE index. value is Bus Maset ID.
	unsigned int mSelfOffAdd; // offset address start position of self area
        unsigned int mSelfSize;	//size of self area
	unsigned int mPeOffAdd; // size of PE offset address 
	ADDRESS_TYPE mBaseAdd; // base address of TPTM register
	unsigned char mIntrvlTmrChNum; //the number of interval timer channel
	
//func
public:
	void start_of_simulation(void); 
	void setThresholdForWrnCmpReg(unsigned int threshold);
#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.
	void setTraceEnable( sc_trace_file* tf );
#endif
private:
	void read_config_file(const char* filename);
	void initParam(void);
};
#endif
