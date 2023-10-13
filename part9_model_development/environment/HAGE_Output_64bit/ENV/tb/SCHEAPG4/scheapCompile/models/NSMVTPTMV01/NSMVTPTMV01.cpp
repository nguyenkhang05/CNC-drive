/*************************************************************************
 *
 * NSMVTPTMV01.cpp
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

#include<string>

#include "CmErrMsg.h"

#include "NSMVTPTMV01.h"
#include "SELF2DEST_BASE.h"
#include "OSCI2DCDR.h"
#include "TPTM.h"
#include "TPTMG.h"

#define TPTM_BASE_ADD 0xFFFBB000
#define TPTM_SELF_OFF_ADD  0x0
#define TPTM_SELF_SIZE 0x100
#define TPTM_PE_OFF_ADD 0x100

using namespace CmErrMsg;

extern int token_to_int(char *token, const char *moduleName, const char *error_message);
extern unsigned int token_to_uint(char *token, const char *moduleName, const char *error_message);
extern unsigned long token_to_ulong(char *token, const char *moduleName, const char *error_message);

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="config_file">configuration file name</param>
NSMVTPTMV01::NSMVTPTMV01(sc_module_name name, const char* config_file):
	G4_CommonPrototype(name, "clk"),
	cpu_clk("cpu_clk"),
	sys_resetn("sys_resetn"),
	TPTM_IRQ(0),
        INTTPTMU(0),
	ts("ts"),
	mpSELF_ADDRESS_DECODER(0),
	mpDECODER(0),
	mpTPTM(0),
	mpTPTMG(0),
	mPeNum(G4TPTM_PE_NUM_INIT),
	mMapFile(),
	mpPeBmid(0),
	mSelfOffAdd(TPTM_SELF_OFF_ADD),
	mSelfSize(TPTM_SELF_SIZE),
	mPeOffAdd(TPTM_PE_OFF_ADD),
	mBaseAdd(TPTM_BASE_ADD),
	mIntrvlTmrChNum(G4TPTM_INTERVAL_TIMER_CH_NUM_INIT)	
{

	G4_CommonPrototype::clk(this->cpu_clk);	
	mPeNum = read_config_file_PeNum(config_file, "NSMVTPTMV01", G4TPTM_PE_NUM_INIT); 
	//Initialize parameter
	initParam (); 
	read_config_file(config_file); 

	mpSELF_ADDRESS_DECODER = new SELF2DEST_BASE("SELF_ADDRESS_DECODER");
	mpDECODER = new OSCI2DCDR<BUS_WIDTH_VCI32>("DECODER", mMapFile);

//port and socket conection
	mpSELF_ADDRESS_DECODER->sys_resetn(this->sys_resetn);
	this->ts(mpSELF_ADDRESS_DECODER->ts);
	mpSELF_ADDRESS_DECODER->is(mpDECODER->ts);
	//Set config file info to TPTM_SELF2DEST_BASE
	mpSELF_ADDRESS_DECODER->setConfig(mPeNum, mpPeBmid,  mSelfOffAdd, mSelfSize, mBaseAdd, mPeOffAdd); 


#ifdef TPTM_CONFIG
	printf("NSMVTPTMV01 config parameter\n");
	printf("\t[PENUM]=%d\n", mPeNum);
	printf("\t[G4TPTM_SELF_AREA]=(0x%x, 0x%x, 0x%x)\n", mSelfOffAdd, mSelfSize, mPeOffAdd);
	printf("\t[BUS_MAP_FILE]=%s\n", mMapFile);
	printf("\t[G4TPTM_BASE_ADDRESS]=0x%x\n", mBaseAdd);
	for (unsigned char  i = 0; i < mPeNum; i++)
	{
		printf("\t[G4CPU_PE_INFO]=(%d, %d)\n", i, mpPeBmid[i] ); 
	}
	printf("\t[G4TPTM_INTERVAL_TIMER_CHNUM]=%d\n",mIntrvlTmrChNum);
	fflush(stdout);
#endif //TPTM_CONFIG
	

		
	//instance and port, socket conection for NSMVTPTMV01
	mpTPTM = new TPTM*[mPeNum]; 	
	this->TPTM_IRQ = new sc_out<bool>*[mPeNum];	
        this->INTTPTMU = new sc_out<bool>**[mPeNum];
	for (unsigned char i = 0; i < mPeNum; i++)
	{
		char str[256] ;
                sprintf(str, "TPTM_IRQ_%d", i);
                this->TPTM_IRQ[i] = new sc_out<bool>(str);
		
		sprintf(str, "TPTM%d", i);
		mpTPTM[i] = new TPTM(str);
		mpTPTM[i]->sys_resetn(this->sys_resetn);
		mpTPTM[i]->clk(this->clk);
		mpDECODER->is(*(mpTPTM[i]->ts));
		mpTPTM[i]->TPTM_IRQ(*(this->TPTM_IRQ[i]));
		INTTPTMU[i] 
			= new sc_out<bool>*[G4TPTM_UP_COUNTER_TIMER_CMP_NUM];
		//set the number of interval timer channel in TPTM
		mpTPTM[i]->setIntrvlTmrChNum(mIntrvlTmrChNum);

                for (unsigned char j = 0; j < G4TPTM_UP_COUNTER_TIMER_CMP_NUM
			; j++)
		{
			sprintf(str, "INTTPTMU%d%d", i, j);
			this->INTTPTMU[i][j] = new sc_out<bool>(str);
			(mpTPTM[i]->INTTPTMU[j])(*(this->INTTPTMU[i][j]));
		}
	}

	mpTPTMG = new TPTMG*[G4TPTM_UP_COUNTER_TIMER_CH_NUM];
	for (unsigned char grobal_ch = 0; grobal_ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; grobal_ch++)
	{
		char str[256] ;
		sprintf(str, "TPTMG%d", grobal_ch);
		mpTPTMG[grobal_ch] = new TPTMG(str);
		mpTPTMG[grobal_ch]->sys_resetn(this->sys_resetn);
		mpTPTMG[grobal_ch]->clk(this->clk);
		mpDECODER->is(*(mpTPTMG[grobal_ch]->ts));
		mpTPTMG[grobal_ch]->setMemVar(mPeNum, grobal_ch, mpTPTM);
#ifdef TPTM_DEBUG_U2A
		printf("TPTMG[%d] name=%s\n", grobal_ch, mpTPTMG[grobal_ch]->name()); fflush(stdout);	
#endif //TPTM_DEBUG_U2A
		for (unsigned char pe = 0; pe < mPeNum; pe++)
		{
			for (unsigned char up_ch = 0; up_ch < G4TPTM_UP_COUNTER_TIMER_CH_NUM; up_ch++)
			{
				mpTPTMG[grobal_ch]->setTPTMnUTRG(pe, up_ch, 
				mpTPTM[pe]->getRegPntr(TPTMSUTRG_OFF_ADD + up_ch));
#ifdef TPTM_DEBUG_U2A
				mpTPTM[pe]->setTPTMG(grobal_ch, mpTPTMG[grobal_ch]);
#endif //TPTM_DEBUG_U2A
			}
		}
		
	}
}

/// <summary>destructor</summary>
NSMVTPTMV01::~NSMVTPTMV01(void) 
{	
	delete mpSELF_ADDRESS_DECODER;
	delete mpDECODER;
	for (unsigned char i = 0; i < mPeNum; i++)
	{	
		delete mpTPTM[i];
		delete TPTM_IRQ[i];
		for (unsigned char j =0; j < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; j++)
		{
			delete INTTPTMU[i][j];
		}
		delete INTTPTMU[i];
	} 
	delete[] mpTPTM;
	for (unsigned char i = 0; i < G4TPTM_UP_COUNTER_TIMER_CH_NUM; i++)
	{	
		delete mpTPTMG[i];
	}
	delete[] mpTPTMG;
	delete[] TPTM_IRQ;
	delete[] INTTPTMU;
	delete[] mpPeBmid;
}

/// <summary>start of simulation</summary>
void NSMVTPTMV01::start_of_simulation( void )
{
	for(unsigned int i = 0; i < mPeNum; i++)
	{
		mpTPTM[i]->ts->setBaseAddressSize
			(mBaseAdd + 0x100 + i * TPTM_PE_OFF_ADD, 
				TPTM_PE_OFF_ADD);
	}
}

///<summary>set Threshold for Waring Message of TPTMmCMPni</summary>
///<param name="threshold">threshold</param>
void NSMVTPTMV01::setThresholdForWrnCmpReg(unsigned int threshold)
{
	if (mpTPTM != NULL)
	{	
		for (unsigned char i = 0; i < mPeNum; i++)
		{
			mpTPTM[i]->setThresholdForWrnCmpReg(threshold);
		}
	}
}

/// <summary>Initialize mpPeBmid</summary>
void NSMVTPTMV01::initParam (void)
{
	mpPeBmid = new unsigned char[mPeNum];
	for (unsigned char i = 0; i < mPeNum; i++)
	{
		mpPeBmid[i] = i;
	}
}
 

///<summary>read the configulation file to get each parameter except the effective PE number</summary>
///<param name="filename">file name of configulation file</param>
void NSMVTPTMV01::read_config_file (const char* filename)
{
	char	  word[512];
	char	  seps[] = " ,=();\t\n\r";
	char	  *token;

	ifstream  config_file(filename);
	
	// read the config file
	while(1) 
	{
		config_file.getline(word, 512, '\n');
		token = strtok(word, seps);
	
		// ----------------------------------------
		// For EOF
		if (config_file.eof()) {
		  break;
		}
		
		// ----------------------------------------
		// For comment
		if ((token == NULL) || (strncmp(token, "//", 2) == 0)) {
		  continue;
		}
		
		// ----------------------------------------
		// detect end marker
		// NOTICE:You have to check whether token is NULL first.
		if (strcmp(token, "[END]") == 0) {
		  break;
		}


		if( strcmp( token, "[BUS_MAP_FILE]" ) == 0 ) 
		{
			token = strtok( 0, seps );
			if (token != NULL)
			{
      				strcpy( mMapFile, token );
			}
#ifdef TPTM_UT_TEST
			else
			{
				printf("NSMVTPTMV01 check NULL for config parameter\n");
				printf("\t[BUS_MAP_FILE] is NULL\n");
				fflush(stdout);

			}
#endif //TPTM_UT_TEST
      			continue;
    		}	


		if (strcmp(token, "[G4TPTM_BASE_ADDRESS]") == 0)
		{	
			token = strtok( 0, seps );
			if (token != NULL)
			{
                 		mBaseAdd =  (ADDRESS_TYPE)(token_to_ulonglong(token, name(), "[G4TPTM_BASE_ADDRESS]"));  
			}
#ifdef TPTM_UT_TEST
			else
			{
				printf("NSMVTPTMV01 check NULL for config parameter\n");
				printf("\t[G4TPTM_BASE_ADDRESS] is NULL\n");
				fflush(stdout);

			}
#endif //TPTM_UT_TEST
      			continue;
		}


		if (strcmp(token, "[G4TPTM_SELF_AREA]") == 0)
		{	
			unsigned int self_area[3] = {
				TPTM_SELF_OFF_ADD,
				TPTM_SELF_SIZE,
				TPTM_PE_OFF_ADD
				};	
			for (unsigned int i = 0; i < 3; i++)
			{
				int result;
				token = strtok(NULL, seps);
				if (token != NULL)
				{
					result = token_to_int(token, name(), "[G4TPTM_SELF_AREA]");
					if (result < 0)
					{	
						char warning_msg_buf[1024];
						sprintf(warning_msg_buf, "[G4TPTM_SELF_AREA] must be more than or equal to 0, but specified value(%d) is changed to default value (%x).(%dst parameter)", result, self_area[i], i + 1);
						printWarningMessage(this->name(), "read_config_file", warning_msg_buf);
					}
					else
					{	
						self_area[i] = (unsigned int)result;	
					}
				}
#ifdef TPTM_UT_TEST
				else
				{
					printf("NSMVTPTMV01 check NULL for config parameter\n");
					printf("\t[G4TPTM_SELF_AREA] %d parameter is NULL\n", i);
					fflush(stdout);

				}
#endif //TPTM_UT_TEST
			}
			mSelfOffAdd = self_area[0];
			mSelfSize = self_area[1];
			mPeOffAdd = self_area[2];
			continue;
		}

		if (strcmp(token, "[G4TPTM_INTERVAL_TIMER_CHNUM]") == 0)
		{
			unsigned int int_ch_num;
			token = strtok(0, seps);
			if (token != NULL)
			{
				int_ch_num = token_to_uint( token, "NSMVG4TPTMV01","[G4TPTM_INTERVAL_TIMER_CHNUM]" );
				if (int_ch_num < 1 || 4 < int_ch_num)
				{
					char error_msg_buf[1024];
					sprintf(error_msg_buf, "[G4TPTM_INTERVAL_TIMER_CHNUM] must be over 1 and under 4, but specified  [%d]." , int_ch_num);
					printErrorMessage(this->name(), "read_config_file", error_msg_buf);
					exit(1);

				}
				mIntrvlTmrChNum = int_ch_num;
			}
#ifdef TPTM_UT_TEST
			else
			{
				printf("NSMVTPTMV01 check NULL for config parameter\n");
				printf("\t[G4TPTM_INTERVAL_TIMER_CHNUM] is NULL\n");
				fflush(stdout);

			}
#endif //TPTM_UT_TEST
      			continue;
		}


		if (strcmp(token, "[G4CPU_PE_INFO]") == 0)
		{	
			unsigned char peid;
			unsigned char bmid;
      			token = strtok(0, seps);
      			if (token != NULL)
			{
      				peid = token_to_uint( token, "NSMVG4TPTMV01","[G4CPU_PE_INFO] pe index(1st parameter)" );
      			  	if( peid > mPeNum - 1)
				{
      			    		char error_message_buf[1024];
      			    		sprintf(error_message_buf,
                            		  "[G4CPU_PE_INFO] pe index must be under %d, but specified [%d]."
                            		  , mPeNum, peid);
			    		// for WinMsg
      			    		printErrorMessage(this->name(), "read_config_file", error_message_buf);
					exit(1);
				}
				else
				{
      			    		token = strtok(0, seps);
					if (token == NULL) 
					{	
#ifdef TPTM_UT_TEST
						printf("NSMVTPTMV01 check NULL for config parameter\n");
						printf("\t[G4CPU_PE_INFO] 2nd parameter is NULL\n");
						fflush(stdout);

#endif //TPTM_UT_TEST
						continue;
					}
      			    		token = strtok(0, seps);
					if (token == NULL) 
					{
#ifdef TPTM_UT_TEST
						printf("NSMVTPTMV01 check NULL for config parameter\n");
						printf("\t[G4CPU_PE_INFO] 3rd parameter is NULL\n");
						fflush(stdout);

#endif //TPTM_UT_TEST
						continue;
					}
					bmid =  token_to_uint( token,
						      "NSMVG4TPTMV01",
						      "[G4CPU_PE_INFO] bus master id(3rd parameter)" );
					if (bmid > 31)
					{	
						char error_message_buf[1024];
	      					sprintf(error_message_buf, "Unexpected token was specified [%s] on [G4CPU_PE_INFO] bus master id(3rd parameter).",
		      				token);
	      					printErrorMessage(name(), "read_config_file", error_message_buf);
						exit(1);
					}
					else
					{
						mpPeBmid[peid] = bmid;
					}
				}
			}
#ifdef TPTM_UT_TEST
			else
			{
				printf("NSMVTPTMV01 check NULL for config parameter\n");
				printf("\t[G4CPU_PE_INFO] 1st parameter is NULL\n");
				fflush(stdout);

			}
#endif //TPTM_UT_TEST
      			continue;
		}
	}
	
	//[BUS_MAP_FILE] is not defined
	if (strlen(mMapFile) == 0)
	{	
		printErrorMessage(name(), "read_config_file", "[BUS_MAP_FILE] is not defined.");
		exit(1);
	}

	//warning for Bus Master ID
	for (unsigned char i = 0; i < mPeNum; i++)
	{
		char error_msg_buf[1024];
		unsigned char chg_pe_id = i;
		for (unsigned char j = i + 1; j < mPeNum; j++)
		{
			if (mpPeBmid[i] == mpPeBmid[j])
			{
				sprintf(error_msg_buf, "There are duplicate [G4CPU_PE_INFO] Bus Master ID(%d) of PE%d and PE%d. (3rd paramter)",mpPeBmid[i] , i, j);
				printErrorMessage(this->name(), "read_config_file", error_msg_buf);
				exit(1);
			}
		}
	}
		
	// end of configuration file read loop(infinite)
}

#if 0
//This function is for checking output signal and using VCD.
//Currently this function is not used.
//This funcfion is remained because it is possible to use this function in the future.

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void NSMVTPTMV01::setTraceEnable( sc_trace_file* tf )
{
	sc_trace( tf, sys_resetn, sys_resetn.name() );
	sc_trace( tf, clk, clk.name() );
	
	for(unsigned char i = 0; i < mPeNum; i++)
	{
		sc_trace( tf, *TPTM_IRQ[i], TPTM_IRQ[i]->name() );		
		mpTPTM[i]->setTraceEnable(tf);
	}
}
#endif
