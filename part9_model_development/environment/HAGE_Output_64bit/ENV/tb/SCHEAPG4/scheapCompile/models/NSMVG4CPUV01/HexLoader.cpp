/*************************************************************************
 *
 *  HexLoader.cpp
 *
 * Copyright(c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno
#include <sstream>
#include <iostream>

#include "HexLoader.h"
#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg

///<summary>constructor</summary>
///<returns>none</returns>
HexLoader::HexLoader( void )
{
}


///<summary>destructor</summary>
///<returns>none</returns>
HexLoader::~HexLoader( void )
{
}


///<summary>set the file pointer of hex file and read the file</summary>
///<param name="filename">file pointer of hex file</param>
///<param name="offset">offset</param>
///<param name="peid">PE ID(PEID-1)</param>
///<returns>none</returns>
void HexLoader::LoadSrec (const char* filename, unsigned int offset, unsigned int peid)
{
  
  std::ifstream loadfs;
  loadfs.open (filename);
  if ( !loadfs ) {
    char error_message_buf[1024];
    sprintf(error_message_buf, 
	    "Program Load Error!\nFile open error: %s", 
	    filename);
    printErrorMessage("HexLoader", "LoadSrec", error_message_buf);
    exit(1);
    return;
  }
  
  std::string buff;
  while ( getline (loadfs, buff) ) {

    if (buff.size() == 0) break;
    if (buff[0] != 'S') continue;

    // S1BBAAAAxxxxxxxxxx....xxxxxxSS
    // S2BBAAAAAAxxxxxxxx....xxxxxxSS
    // S3BBAAAAAAAAxxxxxx....xxxxxxSS
    // S4BBAAAAA..AAAAAxx....xxxxxxSS
    
    unsigned int addr_length;
    switch (buff[1]) {
    case '1': addr_length =  4; break;
    case '2': addr_length =  6; break;
    case '3': addr_length =  8; break;
    case '4': addr_length = 16; break;
    default: continue;
    }

    unsigned int byte_count;
    std::istringstream bcnt_is (buff.substr(2, 2));
    bcnt_is >> std::hex >> byte_count;
    
    unsigned int write_addr;
    std::istringstream waddr_is (buff.substr(4, addr_length));
    waddr_is >> std::hex >> write_addr;
    write_addr += offset;
    
    unsigned int data_pos = addr_length + 4;
    for (unsigned int i = 0; i < byte_count - (addr_length/2) - 1; i++) {
      unsigned int write_data32;
      std::istringstream wdata_is (buff.substr(data_pos, 2));
      wdata_is >> std::hex >> write_data32;
      unsigned char write_data = static_cast<unsigned char> (write_data32);
      
      unsigned int waddr = write_addr + i;
      LoadSrecBody (waddr, write_data, peid);
      data_pos += 2;
    }
  }
  
  loadfs.close ();
}

