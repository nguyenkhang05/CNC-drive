/*************************************************************************
 *
 * NSMVBARRV01.cpp
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

#include<string>

#include "CmErrMsg.h"

#include "NSMVBARRV01.h"
#include "SELF2DEST_BARR.h"
#include "OSCI2DCDR.h"
#include "BARRIER_SYNC.h"

#define BARRIRE_SYNC_BASE_ADD 0xFFFB8000

using namespace CmErrMsg;

extern int token_to_int(char *token, const char *moduleName, const char *error_message);
extern unsigned int token_to_uint(char *token, const char *moduleName, const char *error_message);
extern unsigned long token_to_ulong(char *token, const char *moduleName, const char *error_message);

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
NSMVBARRV01::NSMVBARRV01(sc_module_name name, const char* config_file):
	sc_module(name),
	sys_resetn("sys_resetn"),
	cpu_clk("cpu_clk"),
	ts("ts"),
	mpSELF_ADDRESS_DECODER(0),
	mpDECODER(0),
	mpBARRIER_SYNC(0),
	mPeNum(G4BARR_PE_NUM_INIT),
	mChNum(0),
	mMapFile(),
	mpPeBmid(0),
	mSelfOffAdd(0x100),
	mSelfSize(0x700),
	mPeOffAdd(0x100),
	mBaseAdd(BARRIRE_SYNC_BASE_ADD)
	
{

	
	
	read_config_file_PeNum(config_file); 
	//Initialize parameter
	initParam (); 
	read_config_file(config_file); 

//instance
	mpSELF_ADDRESS_DECODER = new SELF2DEST_BARR("SELF_ADDRESS_DECODER");
	mpDECODER = new OSCI2DCDR<BUS_WIDTH_VCI32>("DECODER", mMapFile);


//port and socket conection
	mpSELF_ADDRESS_DECODER->sys_resetn(this->sys_resetn);
	this->ts(mpSELF_ADDRESS_DECODER->ts);
	mpSELF_ADDRESS_DECODER->is(mpDECODER->ts);
	//Set config file info to SELF2DEST_BARR
	mpSELF_ADDRESS_DECODER->setConfig(mPeNum, mpPeBmid,  mSelfOffAdd, mSelfSize, mBaseAdd, mPeOffAdd); 


#ifdef BARR_CONFIG
	printf("NSMVBARRV01 config parameter\n");
	printf("\t[PENUM]=%d\n", mPeNum);
	printf("\t[G4BARR_CH_NUM]=%d\n", mChNum);
	printf("\t[G4BARR_SELF_AREA]=(0x%x, 0x%x, 0x%x)\n", mSelfOffAdd, mSelfSize, mPeOffAdd);
	printf("\t[BUS_MAP_FILE]=%s\n", mMapFile);
	printf("\t[G4BARR_BASE_ADDRESS]=0x%llx\n", mBaseAdd);
	for (unsigned char  i = 0; i < mPeNum; i++)
	{
		printf("\t[G4CPU_PE_INFO]=(%d, %d)\n", i, mpPeBmid[i] ); 
	}
	fflush(stdout);
#endif //BARR_CONFIG
	

		
	//instance and port, socket conection for NSMVBARRV01IER_SYNC
	mpBARRIER_SYNC = new BARRIER_SYNC*[mChNum]; 	
	
	for (unsigned char i = 0; i < mChNum; i++)
	{
		char str[32] ;
		sprintf(str, "BARRIER_SYNC_%d", i);
		mpBARRIER_SYNC[i] = new BARRIER_SYNC(str);

		mpBARRIER_SYNC[i]->sys_resetn(this->sys_resetn);
		mpBARRIER_SYNC[i]->cpu_clk(this->cpu_clk);
	
		
	
		mpDECODER->is(*(mpBARRIER_SYNC[i]->ts));
		//Set PENUM to BARRIER_SYNC
		mpBARRIER_SYNC[i]->setPeNum(mPeNum); 
	}
	
}

/// <summary>destructor</summary>
NSMVBARRV01::~NSMVBARRV01(void) 
{	
	delete mpSELF_ADDRESS_DECODER;
	delete mpDECODER;
	for (int i = 0; i < mChNum; i++)
	{	
		delete mpBARRIER_SYNC[i];
	} 
	delete[] mpBARRIER_SYNC;
}

/// <summary>start of elaboration</summary>
void NSMVBARRV01::start_of_simulation( void )
{
	for(unsigned int i = 0; i < mChNum; i++)
	{
		mpBARRIER_SYNC[i]->ts->setBaseAddressSize(mBaseAdd + 0x10 * i, 0xf10);
	}
}

/// <summary>Initialize mpPeBmid</summary>
void NSMVBARRV01::initParam (void)
{
	mpPeBmid = new unsigned char[mPeNum];
	for (unsigned char i = 0; i < mPeNum; i++)
	{
		mpPeBmid[i] = i;
	}
}
 
//Now, base address is read by config file.
//In the future, maybe use map file.
#if 0
///<summary>read the configulation file to get base address of barrier-sync register</summary>
///<param name="filename">file name of map file</param>
void NSMVBARRV01::read_map_file (const char* filename)
{
	char      word[512];
        char      seps[] = " ,=();\t\n\r";
        char      *token;

        ifstream  map_file(filename);

	if (map_file.fail())
	{
		char error_msg_buf[1024];
		sprintf(error_msg_buf, "Address map file(%s) cannot be opened", filename);
		printErrorMessage(this->name(), "read_map_file", error_msg_buf);
	
	}	

        // read the config file
        while(1)
        {
                char unrecognized[1024];

                map_file.getline(word, 512, '\n');
                token = strtok(word, seps);

                // ----------------------------------------
                // For EOF
                if (map_file.eof()) {
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
                if (strcmp(token, "[END]") == 0)
		{
                  break;
                }
	
		char tsname[1024];
		sprintf(tsname, "%s.ts", mpSELF_ADDRESS_DECODER->name()); 	
		if (strcmp(token, tsname) == 0)
		{	
			token = strtok( 0, seps );
                        mBaseAdd =  token_to_ulong(token, "read_map_file", name());  
			continue;
		}
	}	
}
#endif

///<summary>read the configulation file to get effective PE number</summary>
///<param name="filename">file name of configulation file</param>
void NSMVBARRV01::read_config_file_PeNum (const char* filename)
{
	char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool      error_detected = false;

  ifstream  config_file(filename);
  // read the config file
  while(1) {
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

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if (strcmp(token, "[PE_NUM]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mPeNum = token_to_uint( token,
				"NSMVG4BARRV01",
				"[PE_NUM]" );
	if(( mPeNum > PE_MAX_NUM ) || ((mPeNum !=1) && ((mPeNum % 2) !=0)) || ( mPeNum == 0 )){
	  char error_message_buf[1024];
	  // for WinMsg
	  sprintf(error_message_buf,
		  "[PE_NUM] must be 1, 2, 4, 6, or 8, but specified [%d]."
		  , mPeNum);
	  printErrorMessage(this->name(), "read_config_file_PeNum", error_message_buf);
	  exit(1);
	}
      }
      continue;
    }
  }  // end of configuration file read loop(infinite)
	
}

///<summary>read the configulation file to get each parameter except the effective PE number</summary>
///<param name="filename">file name of configulation file</param>
void NSMVBARRV01::read_config_file (const char* filename)
{
	char	  word[512];
	char	  seps[] = " ,=();\t\n\r";
	char	  *token;
	unsigned int self_area[3] = {0x100, 0x700, 0x100};
	//flag for being got [G4BARR_SELF_AREA] 
	bool judge_read_self = false;
	//flag for being got [G4BARR_BASE_ADDRESS]	 
	bool exist_base_add = false; 

	ifstream  config_file(filename);
	
	// read the config file
	while(1) 
	{
		//char unrecognized[1024];
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
      			const char *fname = (!token) ? "" : token;
      			strcpy( mMapFile, fname );
      			continue;
    		}	

		if (strcmp(token, "[G4BARR_CH_NUM]") == 0)
		{	
			int result; 
			token = strtok( 0, seps );
                 	result =  token_to_int(token, name(), "[G4BARR_CH_NUM]");  
                        if (result <= 0)
                        {       
				char warning_msg_buf[1024];
                                 sprintf(warning_msg_buf, "[G4BARR_CH_NUM] must be 1 and over, but specified value (%d) is changed to default value (%d).",result, 16);
                                 printWarningMessage(this->name(), "read_config_file", warning_msg_buf);
				mChNum = 16;
			}
			else
			{	mChNum = (unsigned char)result;
			}
		}

		if (strcmp(token, "[G4BARR_BASE_ADDRESS]") == 0)
		{	
			token = strtok( 0, seps );
                 	mBaseAdd =  (ADDRESS_TYPE)(token_to_ulonglong(token, name(), "[G4BARR_BASE_ADDRESS]"));  
			exist_base_add = true;
		}


		if (strcmp(token, "[G4BARR_SELF_AREA]") == 0)
		{	
			for (unsigned int i = 0; i < 3; i++)
			{	
				int result;
				result = token_to_int(strtok(NULL, seps), name(), "[G4BARR_SELF_AREA]");
				if (result < 0)
				{	
					char warning_msg_buf[1024];
					sprintf(warning_msg_buf, "[G4BARR_SELF_AREA]   must be 0 and over, but specified value(%d) is changed to default value (0x%x).(%d parameter)", result, self_area[i], i + 1);
					printWarningMessage(this->name(), "read_config_file", warning_msg_buf);
				}
				else
				{	
					self_area[i] = (unsigned int)result;
				}
			}
			mSelfOffAdd = self_area[0];
			mSelfSize = self_area[1];
			mPeOffAdd = self_area[2];
			judge_read_self = true; //flag
			continue;
		}

		if (strcmp(token, "[G4CPU_PE_INFO]") == 0)
		{	
			 unsigned char peid;
			unsigned char bmid;
      			token = strtok(0, seps);
      			if (token != NULL)
			{
      				peid = token_to_uint( token, "NSMVG4BARRV01","[G4CPU_PE_INFO] pe index(1st parameter)" );
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
      			    		token = strtok(0, seps);
					bmid =  token_to_uint( token,
						      "NSMVG4BARRV01",
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
      			continue;
		}


//Delete this function(G4BARR_MODE_BMID_PE).Because functional specifcation was changed.
#if 0		
		if (strcmp(token, "[G4BARR_MODE_BMID_PE]") == 0)
		{	
			bool bmid_exist = false; //flag for Bus Master ID exist in [G4CPU_PE_INFO] 
			char warning_msg_buf[1024];
			unsigned char result;
			result = (unsigned char)token_to_uint(strtok(NULL, seps), name(), "[G4BARR_MODE_BMID_PE]");
			for (unsigned char i = 0; i < mPeNum; i++)
			{
				if (mpPeBmid[i] == result)
				{	
					bmid_exist = true;
				}
			}
			if (bmid_exist)
			{
					tmp_bmid_valid[result] = strtok(0, seps);
			}
			else
			{
				sprintf(warning_msg_buf, "[G4BARR_MODE_BMID_PE] Bus Master ID doesn't exist on [G4CPU_PE_INFO] Bus Master ID, but specified (%d) is not set. (1st parameter)", result);
				printWarningMessage(name(), "read_config_file", warning_msg_buf);
			}
			continue;
		}
#endif

	}
	
	//[BUS_MAP_FILE] is not defined
	if (strlen(mMapFile) == 0)
	{	
		printErrorMessage(name(), "read_config_file", "[BUS_MAP_FILE] is not defined.");
		exit(1);
	}


	//If [G4BARR_SELF_AREA] is not set, mSelfOffAdd and mSelfSize will be set to default value
	if (! judge_read_self)
	{
		mSelfOffAdd = 0x100;
		mSelfSize = 0x700;
		mPeOffAdd = 0x100;
	}

	//Check the number of channels 
	if (mChNum == 0) 
	{
		mChNum = 16;
	}
	if (mChNum > 16) 
	{	
               char warning_msg_buf[1024];
               sprintf(warning_msg_buf, "[G4BARR_CH_NUM]  must be under %d, but specified %d is changed to default value (%d)",16  ,mChNum, 16);
               printWarningMessage(this->name(), "read_config_file", warning_msg_buf);
              mChNum = 16; 
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
		
//Delete this function(G4BARR_MODE_BMID_PE).Because functional specifcation was changed.
#if 0
		if (tmp_bmid_valid.find(mpPeBmid[i]) != tmp_bmid_valid.end() ) //Bus Master ID (i) exist
		{
			if (strcmp(tmp_bmid_valid[mpPeBmid[i]].c_str(), "true") == 0)
			{
				mBmidValid[mpPeBmid[i]] = true;	
			}
			else if (strcmp(tmp_bmid_valid[mpPeBmid[i]].c_str(), "false") == 0)
			{
				mBmidValid[mpPeBmid[i]] = false;	
			}
			else //character string of Bus Master ID valid is not ture or false
			{	
				sprintf(error_msg_buf, "[G4BARR_MODE_BMID_PE] Bus Master ID (%d) valid must be true or false, but specified value(%s) is changed to default value (true).(2rd parameter)", mpPeBmid[i], tmp_bmid_valid[mpPeBmid[i]].c_str());
				printWarningMessage(this->name(), "read_config_file", error_msg_buf);
				mBmidValid[mpPeBmid[i]] = true;	
			}
		}
		else //Bus Master ID (i) don't exist
		{
			mBmidValid[mpPeBmid[i]] = true;
		}
#endif 
	// end of configuration file read loop(infinite)
}
