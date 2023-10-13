/*************************************************************************
 *
 * NSMVBARRV01.h
 *
 * Copyright(c) 2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#ifndef __NSMVBARRV01__
#define __NSMVBARRV01__

#include "OSCI2.h"
#include "global.h"
#include "Common.h"

#include<map>

using namespace std;

class BarrierSyncFunc;
class SELF2DEST_BARR;
class BARRIER_SYNC;
template< unsigned int BUSWIDTH > class OSCI2DCDR;


class NSMVBARRV01 
: public sc_module
{
public:

//constructor destructor
	NSMVBARRV01(sc_module_name name = "BARR", const char* config_file = NULL); 
	~NSMVBARRV01(void);
	void start_of_simulation(void); 


//varriable
	sc_in<bool> sys_resetn; // reset port
	sc_in<sc_dt::uint64> cpu_clk; // clock port

	TlmTargetSocket<BUS_WIDTH_VCI32> ts; // target socket 
	
	SELF2DEST_BARR *mpSELF_ADDRESS_DECODER; // pointer of SELF2DEST_BARR instance 
	OSCI2DCDR<BUS_WIDTH_VCI32> *mpDECODER; // pointer of OSCI2DCDR instance 
	BARRIER_SYNC **mpBARRIER_SYNC; //pointer of BARRIER_SYNC instance

private :
	unsigned char mPeNum; // the number of PE
	unsigned char mChNum; // the number of Barrier-Sync's channel 
	char mMapFile[1024]; // map file name
	unsigned char *mpPeBmid; // argument is PE index. value is Bus Maset ID.
	//map<unsigned char, bool> mBmidValid; // argument is Bus Master ID. value is valid(true) or invalid(false).
	unsigned int mSelfOffAdd; // offset address start position of self area
        unsigned int mSelfSize;	//size of self area
	unsigned int mPeOffAdd; // size of PE offset address 
	ADDRESS_TYPE mBaseAdd; // base address of BarrierSync register
	
//func
	void read_config_file(const char* filename);
	void read_config_file_PeNum(const char* filename);
	//void read_map_file (const char* filename);
	void initParam(void);
};
#endif
