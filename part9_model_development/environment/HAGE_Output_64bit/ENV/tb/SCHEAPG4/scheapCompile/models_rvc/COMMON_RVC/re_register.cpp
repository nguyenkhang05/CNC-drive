/*************************************************************************
 *
 *  re_register.cpp
 *
 * Copyright(c) 2010-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
*************************************************************************/
/// @re_register.cpp
/// @brief General register class
/// $Id: re_register.cpp v2016_09_21 $
/// $Date$
/// $Revison$
/// $Author$

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cassert>
#include "re_register.h"
#ifdef REGIF_SC_REPORT
#include "systemc.h"
#endif//REGIF_SC_REPORT
using namespace std;

#ifndef message
#define message get_fileline(__FILE__, __LINE__); _message
#endif//message

#ifndef bit_info_msg
#define bit_info_msg mParent->get_fileline(__FILE__, __LINE__);mParent->_message
#endif//bit_info_msg
//
// bit_info class
//

//
// bit setting
//
// -----------------------------------------------------------------------------
// Get reigster's address and parent' pointer to use call-back function
// -----------------------------------------------------------------------------
vpcl::bit_info::bit_info(unsigned int addr, re_register *parent)
{                                                                         //{{{1
  if (parent == NULL) {
    printf("Cannot allocate memory at %d (%s)\n", __LINE__, __FILE__);
    return;
  }

  mAddr             = addr;
  mParent           = parent;
  mSurpressMessage  = true;
  mModeNum          = 0;

  for (unsigned int i=0 ; i<MAX_OWNER ; i++) {
    mMode[i] = NOT_ENTRY;
  }
  next = NULL;
}                                                                         //}}}1

//
// bit access // phase 2 added
//
// -----------------------------------------------------------------------------
// Assignment to the all bit
// -----------------------------------------------------------------------------
vpcl::bit_info &vpcl::bit_info::operator = (bit_info org)
{                                                                         //{{{1
  this->mEndAddr         = org.mEndAddr;                 
  this->mStartAddr       = org.mStartAddr;               
  this->mBitName         = org.mBitName;                 
  this->mInitVal         = org.mInitVal;                
  for (unsigned int i = 0; i< MAX_OWNER; i++) {
    this->mMode[i] = org.mMode[i];         
  }
  this->mSpp             = org.mSpp;                     
  this->mWritableType    = org.mWritableType;           
  this->mWritableList.assign(org.mWritableList.begin(), org.mWritableList.end());            
  this->mAccessStatus    = org.mAccessStatus;           
  return *this;
}                                                                         //}}}1

//
// bit access
//
// -----------------------------------------------------------------------------
// Assignment to the each bit
// -----------------------------------------------------------------------------
vpcl::bit_info &vpcl::bit_info::operator = (unsigned int data)
{                                                                         //{{{1

  if (mSurpressMessage == false) { // writing access 

    bool wr_success = false;
    unsigned int chk_num = (mMode[mModeNum] == NOT_ENTRY)? 0 : mModeNum;

    if (data == 0) {
      switch (mMode[chk_num]) {
        case emAccR:
        case emAccW1:
        case emAccW1S0:
        case emAccRW1S0: // added (phase 5)
        case emAccW1BR:
        case emAccW1S0BR:
        case emAccRW1:
        case emAccRBW1:
        case emAccRBW1S0:
        case emAccW1T: // added 2015.Feb.11
        case emAccRW1T:
        case emAccRS0:
        case emAccRS0W1:
        case emAccRS1:
        case emAccRS1W1:
          bit_info_msg(emWarningLevel, "%s forbids to write 0x%x", mBitName.c_str(), data);
          return *this;

        case emAccRBW:
          if (mAccessStatus != emReadStatus) {
            bit_info_msg(emErrorLevel, "Read action is required before writing 0x%X to %s", data, mBitName.c_str());
            return *this;
          }
          wr_success = WriteValue(0);
          break;

        case emAccRBW0:
        case emAccRBW0S1:
          if (mAccessStatus != emReadStatus) {
            bit_info_msg(emErrorLevel, "Read action is required before writing 0 to %s", mBitName.c_str());
            return *this;
          }
          if (mMode[chk_num] == emAccRBW0S1) {
            wr_success = WriteValue(1);        // emAccRBW0S1
          } else {
            wr_success = WriteValue(0);        // emAccRBW0
          }
          break;

        case emAccW0S1:
        case emAccW0S1BR:
        case emAccWS1: // added 2015.Feb.11
        case emAccRWS1:
        case emAccRW0S1:
          SetValue(1);
          wr_success = true;
          break;

        case emAccD:
          if (mParent->mParent->ChkAccess(true,"",mParent->name(),mBitName.c_str())) {
            wr_success = WriteValue(0);
            break;
          }
          else {
            bit_info_msg(emWarningLevel, "%s is not allowed to access this time", mBitName.c_str());
            return *this;
          }

        case emAccW0T:
        case emAccRW0T:
          if (mParent->mParent->rd_cb(mAddr) == 0) {
            wr_success = WriteValue(1);
          } else {
            wr_success = WriteValue(0);
          }
          break;

        default: // emAccRW, emAccW, emAccW0, emAccRW0, emAccWBR, emAccW0BR
                 // emAccWS0, emAccRWS0, emAccRS0W, emAccRS0W0, emAccRS1W, emAccRS1W0
          wr_success = WriteValue(0);
          break;
      }
    }
    else { // written data != 0
      switch (mMode[chk_num]) {
        case emAccR:
        case emAccW0:
        case emAccW0S1:
        case emAccW0BR:
        case emAccW0S1BR:
        case emAccRW0:
        case emAccRBW0:
        case emAccRBW0S1:
        case emAccW0T: // added 2015.Feb.11
        case emAccRW0T: 
        case emAccRW0S1:
        case emAccRS0:
        case emAccRS1:
        case emAccRS0W0:
        case emAccRS1W0:
          bit_info_msg(emWarningLevel, "%s forbids to write 0x%x", mBitName.c_str(), data);
          return *this;

        case emAccRBW1:
        case emAccRBW1S0:
          if (mAccessStatus != emReadStatus) {
            bit_info_msg(emErrorLevel, "Read action is required before writing 1 to %s", mBitName.c_str());
            return *this;
          }
          if (mMode[chk_num] == emAccRBW1S0) {
            wr_success = WriteValue(0);        // emAccRBW1S0
          } else {
            wr_success = WriteValue(data);     // emAccRBW1
          }
          break;

        case emAccRBW:
          if (mAccessStatus != emReadStatus) {
            bit_info_msg(emErrorLevel, "Read action is required before writing 0x%X to %s", data, mBitName.c_str());
            return *this;
          }
          wr_success = WriteValue(data);
          break;

        case emAccW1S0:
        case emAccRW1S0: // added (phase 5)
        case emAccW1S0BR:
        case emAccWS0:
        case emAccRWS0:
          SetValue(0);
          wr_success = true;
          break;

        case emAccWS1:
          SetValue(1);
          wr_success = true;
          break;

        case emAccD:
          if (mParent->mParent->ChkAccess(true,"",mParent->name(),mBitName)) {
            wr_success = WriteValue(data);
            break;
          }
          else {
            bit_info_msg(emWarningLevel, "%s is not allowed to access this time", mBitName.c_str());
            return *this;
          }
        case emAccW1T:
        case emAccRW1T:
          if (mParent->mParent->rd_cb(mAddr) == 0) {
            wr_success = WriteValue(1);
          } else {
            wr_success = WriteValue(0);
          }
          break;

        default: // emAccRW, emAccW, emAccW1, emAccRW1, emAccWBR, emAccW1BR
                 // emAccRWS1, emAccRS0W, emAccRS0W1, emAccRS1W, emAccRS1W1
          wr_success = WriteValue(data);
          break;
      }
    }

    // set access status
    if (wr_success) {
      if (data == 0){
        mAccessStatus = emWritten0Status;
      } else if (data == 1) {
        mAccessStatus = emWritten1Status;
      } else {
        mAccessStatus = emWrittenStatus;
      }
    }

    switch (mSpp) {
      case SPP_DISABLE:
        if (data != 0){
          bit_info_msg(emInfoLevel, "%s won't affect to operation", mBitName.c_str());
        }
         break;
      default : // SPP_ENABLE
         break;
    }

    // return after writing
    return *this;
  }

  // set value in case of bit assignment
  SetValue(data);
  return *this;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Write value to bit
// -----------------------------------------------------------------------------
bool vpcl::bit_info::WriteValue(const unsigned int value)
{                                                                         //{{{1
  // writable type is emValueAny
  if (mWritableType == emValueAny) {
    SetValue(value);
    return true;
  }

  // writable type is emValueRange
  if (mWritableType == emValueRange) {
    unsigned int &min_val = mWritableList.front();
    unsigned int &max_val = mWritableList.back();
    if ((value >= min_val) && (value <= max_val)) {
      SetValue(value);
      return true;
    }
    else {
      bit_info_msg(emErrorLevel, "Written value %d is out of writable value list when writing to %s", value, mBitName.c_str());
      return false;
    }
  }

  // writable type is emValueList
  for (unsigned int i=0; i<mWritableList.size(); i++) {
    if (value == mWritableList[i]) {
      SetValue(value);
      return true;
    }
  }
  bit_info_msg(emErrorLevel, "Written value %d is out of writable value list when writing to %s", value, mBitName.c_str());
  return false;

}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Set bit value
// -----------------------------------------------------------------------------
void vpcl::bit_info::SetValue(const unsigned int data)
{                                                                         //{{{1
  // get current value from reg_super
  unsigned int val = mParent->mParent->rd_cb(mAddr);
  unsigned int bit_data;

  // bit clear at specified area to write
  if (mEndAddr-mStartAddr+1 == REG_SIZE) {
    val = 0x00000000;
    bit_data = data;
  }
  else {
    val &= ~(((1<<(mEndAddr-mStartAddr+1))-1) << mStartAddr);
    bit_data = data & ((1<<(mEndAddr-mStartAddr+1))-1);
  }

  // bit set to write specified value
  val |= (bit_data<<mStartAddr);

  // return value to reg_super
  mParent->mParent->wr_cb(mAddr, val);
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Get bit value 
// -----------------------------------------------------------------------------
unsigned int vpcl::bit_info::read() const
{                                                                         //{{{1
  // get the whole of register value from reg_super
  unsigned int val = mParent->mParent->rd_cb(mAddr);

  // pick up target area
  if (mEndAddr-mStartAddr+1 != REG_SIZE) {
    val >>= mStartAddr;
    val &= ((1<<(mEndAddr-mStartAddr+1))-1);
  }

  return val;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return each bit value
// -----------------------------------------------------------------------------
vpcl::bit_info::operator unsigned int ()
{                                                                         //{{{1
  return this->read();
}                                                                         //}}}1

//
// multi master
//
// -----------------------------------------------------------------------------
// Specify additional access condition
// -----------------------------------------------------------------------------
void vpcl::bit_info::add_condition(unsigned int owner, int mode)
{                                                                         //{{{1
  if (owner >= MAX_OWNER) {
    bit_info_msg(emErrorLevel, "Cannot entry access mode more than %d owner", MAX_OWNER);
    return;
  }

  mMode[owner] = mParent->GetAccessMode(mode);
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Specify access number
// -----------------------------------------------------------------------------
void vpcl::bit_info::access_owner(unsigned int num)
{                                                                         //{{{1
  mModeNum = num;
}                                                                         //}}}1

//
// other
//
// -----------------------------------------------------------------------------
// Return bit name
// -----------------------------------------------------------------------------
std::string vpcl::bit_info::name()
{                                                                         //{{{1
  return mBitName;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Control put or not put error message
// -----------------------------------------------------------------------------
void vpcl::bit_info::surpress_message(bool control)
{                                                                         //{{{1
  mSurpressMessage = control;
}                                                                         //}}}1

//
// re_register class
//

//
// register setting
//
// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
vpcl::re_register::re_register()
{                                                                         //{{{1
  InitMsgLevel();
  mInitialized = false;
  mBitInfo     = NULL;
  mIsReset = false;
  mFileName = "";
  mLineNum = 0;
}

vpcl::re_register::re_register(const re_register &org)
{
  InitMsgLevel();
  // create new copy object
  mBitInfo = new bit_info(org.mAddr, this);
  mBitInfo->next = NULL;
  mIsReset = false;
  mFileName = "";
  mLineNum = 0;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
vpcl::re_register::~re_register()
{                                                                         //{{{1
  // when any bit is not assigned
  if (mBitInfo == NULL) {
    return;
  }

  // delete each bit
  while (mBitInfo->next != NULL) {
    bit_info *it = mBitInfo->next;
//    mBitInfo->mWritableList.clear();
    delete mBitInfo;
    mBitInfo  = it;
  }

  if (mBitInfo != NULL) {
//    mBitInfo->mWritableList.clear();
    delete mBitInfo;
    mBitInfo = NULL;
  }
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Copy operator
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator=(const re_register &org)
{                                                                         //{{{1
  if (this == &org) {
    return *this;
  }
// phase 2 added --- 
  //  delete mBitInfo;
  while (mBitInfo->next != NULL) {
    bit_info *it = mBitInfo->next;
    delete mBitInfo;
    mBitInfo  = it;
  }

  if (mBitInfo != NULL) {
    delete mBitInfo;
    mBitInfo = NULL;
  }
  // create new copy object
  mBitInfo = new bit_info(org.mAddr, this);
  *mBitInfo = *(org.mBitInfo);
  
  bit_info* org_bit = org.mBitInfo;
  bit_info* new_bit = mBitInfo;
  if ((org_bit != NULL) && (new_bit != NULL)) { 
      while(org_bit->next != NULL) {
        bit_info* bf = new bit_info(org.mAddr, this);
        *bf = *org_bit;
        new_bit->next = bf;
        new_bit = bf->next;
        org_bit = org_bit->next;
      }
  }
// --- 
  this->mParent      = org.mParent;
  this->mAddr        = org.mAddr;
  this->mModName     = org.mModName;
  this->mRegName     = org.mRegName;
  this->mEntried     = org.mEntried;
  this->mAccessNum   = org.mAccessNum;
  this->mInitialized = org.mInitialized;

  return *this;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Get register's address, parent's pointer, and register/model name
// -----------------------------------------------------------------------------
vpcl::re_register::re_register(
    unsigned int  addr
  , reg_super    *parent
  , std::string   reg_name
  , const char   *mod_name)
{                                                                         //{{{1
  InitMsgLevel();
  set(addr, parent, reg_name, mod_name);
  mIsReset = false;
  mFileName = "";
  mLineNum = 0;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Register whole setting
// -----------------------------------------------------------------------------
void vpcl::re_register::set(
    unsigned int  addr
  , reg_super    *parent
  , std::string   reg_name
  , const char   *mod_name)
{
  mBitInfo   = NULL;
  mAddr      = addr;
  mEntried   = 0;
  mAccessNum = 0;
                                                                        //{{{1
  if ((parent == NULL) || (reg_name == "") || (mod_name == NULL)) {
    message(emErrorLevel, "Cannot allocate memory at %d (%s)", __LINE__, __FILE__);
    return;
  }

  mParent    = parent;
  mRegName   = reg_name;
  mModName   = mod_name;

  mInitialized = true;
}                                                                         //}}}1

//
// bit setting
//
// -----------------------------------------------------------------------------
// Specify each bit information(multi address)
// -----------------------------------------------------------------------------
void vpcl::re_register::operator ()(
    unsigned int  eaddr
  , unsigned int  saddr
  , std::string   bit_name
  , unsigned int  init_val
  , int           mode
  , eSupportMode  spp
  , bool          overlap)
{                                                                         //{{{1
  // check valid name and address
  if (CheckError(eaddr,saddr,bit_name,overlap)) {
    return;
  }

  // get access mode
  int acc_mode = GetAccessMode(mode);

  // create new bit
  std::vector<unsigned int> val_list;
  CreateBit(eaddr,saddr,bit_name,init_val,acc_mode,spp,emValueAny,val_list);
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Specify each bit information(single address)
// -----------------------------------------------------------------------------
void vpcl::re_register::operator ()(
    unsigned int  eaddr
  , std::string   bit_name
  , unsigned int  init_val
  , int           mode
  , eSupportMode  spp)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return;
  }

  this->operator()(eaddr, eaddr, bit_name, init_val, mode, spp);
}                                                                         //}}}1
//
// bit setting for enhancement
//
// -----------------------------------------------------------------------------
// Specify each bit information(multi address)
// -----------------------------------------------------------------------------
void vpcl::re_register::operator ()(
    unsigned int  eaddr
  , unsigned int  saddr
  , std::string   bit_name
  , unsigned int  init_val
  , std::string   mode
  , eSupportMode  spp
  , std::string   writable_list
  , bool          overlap)
{                                                                         //{{{1
  // check valid name and address
  if (CheckError(eaddr,saddr,bit_name, overlap)) {
    return;
  }

  // get access mode
  eAccessMode acc_mode = GetAccessMode(mode);
  if (acc_mode == emNotAccessMode) {
    return;
  }

  // get writable type and value list
  std::vector<unsigned int> val_list;
  eWritableType wr_type = GetWritableType(writable_list,val_list);

  if (wr_type == emValueError) {
    return;
  }

  // create new bit
  CreateBit(eaddr,saddr,bit_name,init_val,acc_mode,spp,wr_type,val_list);
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Specify each bit information(single address)
// -----------------------------------------------------------------------------
void vpcl::re_register::operator ()(
    unsigned int  eaddr
  , std::string   bit_name
  , unsigned int  init_val
  , std::string   mode
  , eSupportMode  spp
  , std::string   writable_list)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return;
  }

  this->operator()(eaddr, eaddr, bit_name, init_val, mode, spp, writable_list);
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Check input information for bit setting
// -----------------------------------------------------------------------------
bool vpcl::re_register::CheckError(unsigned int eaddr, unsigned int saddr, std::string bit_name, bool overlap)
{                                                                         //{{{1

  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return true;
  }

  if (bit_name == "") {
    message(emErrorLevel, "Cannot allocate memory at %d (%s)", __LINE__, __FILE__);
    return true;
  }

  // address size check
  if (eaddr < saddr) {
    message(emErrorLevel, "End address(%d) is smaller than start address(%d)", eaddr, saddr);
    return true;
  }

  if (eaddr > 31) {
    message(emErrorLevel, "End address(%d) is larger than 31", eaddr);
    return true;
  }

  // entried area check
  unsigned current_entry;
  if (eaddr == 31 && saddr == 0) {
    current_entry = 0xFFFFFFFF;
  }
  else {
    current_entry = ((1<<(eaddr-saddr+1))-1)<<saddr;
  }

  if (!overlap) {
    if( (current_entry&mEntried) != 0 ) {
      message (emErrorLevel, "(%d - %d) is entried area", saddr,eaddr);
      return true;
    }
    else {
      mEntried |= current_entry;
    }
  }

  // entried bit name check
  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    if(bit_name == it->mBitName) {
      message(emErrorLevel, "%s is used yet(%d-%d)", bit_name.c_str(), it->mEndAddr, it->mStartAddr);
      return true;
    }
  }

  return false;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Get access mode from input integer
// -----------------------------------------------------------------------------
int vpcl::re_register::GetAccessMode(int mode)
{                                                                         //{{{1
  int acc_mode = 0;

  // support legacy
  if (mode == (ACC_W0|ACC_W1)) {
    acc_mode = emAccW;
  }
  else if (mode == (ACC_RD|ACC_W0|ACC_W1)) {
    acc_mode = emAccRW;
  }
  else if (mode == (ACC_RD|ACC_W0)) {
    acc_mode = emAccRW0;
  }
  else if (mode == (ACC_RD|ACC_W1)) {
    acc_mode = emAccRW1;
  }
  else {
    switch(mode) {
      case ACC_WRITE_ZERO_ONLY:
      case ACC_W0:
        acc_mode = emAccW0; break;

      case ACC_WRITE_ONE_ONLY:
      case ACC_W1 :
        acc_mode = emAccW1; break;

      case ACC_WRITE_ONLY:
        acc_mode = emAccW;  break;

      case ACC_READ_ONLY:
      case ACC_RD:
        acc_mode = emAccR;  break;

      case ACC_READ_WRITE:
      case ACC_RW:
        acc_mode = emAccRW; break;

      default:
        acc_mode = mode;    break;
    }
  }

  return acc_mode;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Get access mode from input string
// -----------------------------------------------------------------------------
vpcl::eAccessMode vpcl::re_register::GetAccessMode(const std::string mode)
{                                                                         //{{{1

std::string AccModeStr[N_SPP_MODE] = {"D","R","W","W0|W1","W1|W0","W0","W1","R|W","W|R","R-W0","R-W1","R-W","W-R","W0:1","W1:0","R|W1:0",
                                      "R|W0|W1","R|W1|W0","W0|R|W1","W0|W1|R","W1|R|W0","W1|W0|R","R|W0","R|W1","W0|R","W1|R",
                                      "R-W0:1", "R-W1:0","W0-R","W1-R","W0:1-R","W1:0-R",
                                      "W1:T", "W0:T", "R|W1:T", "R|W0:T", // added 2015.Feb.11
                                      "W:0", "W:1", "R|W:0", "R|W:1", "R|W0:1", "R:0", "R:0|W", "R:0|W0", "R:0|W1", 
                                      "R:1", "R:1|W", "R:1|W0", "R:1|W1"};
eAccessMode AccModeArr[N_SPP_MODE] = {emAccD,emAccR,emAccW,emAccW,emAccW,emAccW0,emAccW1,emAccRW,emAccRW,emAccRBW0,emAccRBW1,emAccRBW,emAccWBR,emAccW0S1,emAccW1S0,emAccRW1S0,
                                      emAccRW,emAccRW,emAccRW,emAccRW,emAccRW,emAccRW,emAccRW0,emAccRW1,emAccRW0,emAccRW1,
                                      emAccRBW0S1,emAccRBW1S0,emAccW0BR,emAccW1BR,emAccW0S1BR,emAccW1S0BR,
                                      emAccW1T,emAccW0T,emAccRW1T,emAccRW0T, // added 2015.Feb.11
                                      emAccWS0,emAccWS1,emAccRWS0,emAccRWS1,emAccRW0S1,emAccRS0,emAccRS0W,emAccRS0W0,emAccRS0W1,
                                      emAccRS1,emAccRS1W,emAccRS1W0,emAccRS1W1};

  for (unsigned int index=0; index < N_SPP_MODE; index++) {
    if (mode == AccModeStr[index]) {
      return AccModeArr[index];
    }
  }

  message(emErrorLevel, "Access mode %s is not supported",mode.c_str());
  return emNotAccessMode;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Get writable type and value list
// -----------------------------------------------------------------------------
vpcl::eWritableType vpcl::re_register::GetWritableType(const std::string writable_list, std::vector<unsigned int> &val_list)
{                                                                         //{{{1
  if (writable_list == "") {
    return emValueAny;
  }

  unsigned int value = 0;
  size_t pos = 0;

  if ((pos = writable_list.find('-')) != std::string::npos) { // writable list specified as a range
    // get minimum value and maximum value
    std::string fst_str = writable_list.substr(0,pos);
    std::string snd_str = writable_list.substr(pos+1);
    // check and insert to value list
    if (IsNumber(fst_str) && IsNumber(snd_str)) {
      value = Str2Int(fst_str);
      unsigned int tmp = Str2Int(snd_str);
      // arrange value list
      if (value <= tmp) {
        val_list.push_back(value);
        val_list.push_back(tmp);
      }
      else {
        val_list.push_back(tmp);
        val_list.push_back(value);
      }
      return emValueRange;
    }
    else {
      message(emErrorLevel, "Specify wrong format for writable value");
      return emValueError;
    }
  }

  // split writable value list by comma
  std::vector<std::string> str_list;
  Split(writable_list,',',str_list);
  for (std::vector<std::string>::iterator iter = str_list.begin(); iter != str_list.end(); iter++) {
    if (IsNumber(*iter)) {
      value = Str2Int(*iter);
      val_list.push_back(value);
    }
    else {
      message(emErrorLevel, "Specify wrong format for writable value");
      return emValueError;
    }
  }

  return emValueList;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Check input string is number or not
// -----------------------------------------------------------------------------
bool vpcl::re_register::IsNumber(const std::string num_str)
{                                                                         //{{{1
  std::string sub_str;

  if ((sub_str = num_str.substr(0,2)) == "0x" || (sub_str == "0X")) { // hexadecimal
    return (IsDigitStr(2,num_str.substr(2)));
  }

  if ((sub_str = num_str.substr(0,1)) == "b" || (sub_str == "B")) { // binary
    return (IsDigitStr(0,num_str.substr(1)));
  }

  return (IsDigitStr(1,num_str));
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Check valid digit
// -----------------------------------------------------------------------------
bool vpcl::re_register::IsDigitStr(const int type, const std::string dig_str)
{                                                                         //{{{1
  if (dig_str == "") {
    return false;
  }

  char ch = 0;

  switch (type) {
    case 0: // binary digit string
      for (unsigned i=0; i<dig_str.length(); i++) {
        ch = dig_str[i];
        if ((ch < '0') || (ch > '1')) {
          return false;
        }
      }
      break;

    case 1: // decimal digit string
      for (unsigned i=0; i<dig_str.length(); i++) {
        ch = dig_str[i];
        if ((ch < '0') || (ch > '9')) {
          return false;
        }
      }
      break;

    default: // hexadecimal digit string
      for (unsigned i=0; i<dig_str.length(); i++) {
        ch = dig_str[i];
        if (((ch < '0') || (ch > '9')) && ((ch < 'A') || (ch > 'F')) && ((ch < 'a') || (ch > 'f'))) {
          return false;
        }
      }
      break;
  }

  return true;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Convert a string to a number
// -----------------------------------------------------------------------------
unsigned int vpcl::re_register::Str2Int(const std::string num_str)
{                                                                         //{{{1
  std::string sub_str;

  if ((sub_str = num_str.substr(0,2)) == "0x" || (sub_str == "0X")) { // hexadecimal
    return (GetValue(1,num_str.substr(2)));
  }

  if ((sub_str = num_str.substr(0,1)) == "b" || (sub_str == "B")) { // binary
    return (GetValue(0,num_str.substr(1)));
  }

  return (unsigned int)atoi(num_str.c_str());
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Get integer value from a string
// -----------------------------------------------------------------------------
unsigned int vpcl::re_register::GetValue(const int type, const std::string dig_str)
{                                                                         //{{{1
  int str_len = dig_str.length();
  unsigned int pow_fac = 1;
  unsigned int value   = 0;
  char ch = 0;

  switch (type) {
    case 0: // binary digit string
      for (int i=str_len-1; i>=0; i--) {
        ch = dig_str[i];
        value += (ch-'0')*pow_fac;
        pow_fac *= 2;
      }
      break;

    default: // hexadecimal digit string
      for (int i=str_len-1; i>=0; i--) {
        ch = dig_str[i];
        if (ch <= '9') {
          ch -= '0';
        }
        else if (ch <= 'F') {
               ch -= 55;
             }
             else {
               ch -= 87;
             }
        value += ch*pow_fac;
        pow_fac *= 16;
      }
      break;
  }

  return value;
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Split string by delimiter
// -----------------------------------------------------------------------------
void vpcl::re_register::Split(const std::string writable_list, const char delimiter, std::vector<std::string> &str_list)
{                                                                         //{{{1
  size_t pos = 0;
  std::string str = writable_list;
  std::string val_str;

  while (pos != std::string::npos) {
    // separate strings
    pos = str.find(delimiter);
    if (pos != std::string::npos) {
      val_str = str.substr(0,pos);
      str = str.substr(pos+1);
    }
    else {
      val_str = str;
    }
    // insert to string list
    if (val_str != "") {
      str_list.push_back(val_str);
    }
  }
}                                                                         //}}}1
// -----------------------------------------------------------------------------
// Create bit
// -----------------------------------------------------------------------------
void vpcl::re_register::CreateBit(
  unsigned int    eaddr
  , unsigned int  saddr
  , std::string   bit_name
  , unsigned int  init_val
  , int           mode
  , eSupportMode  spp
  , eWritableType value_type
  , std::vector<unsigned int> writable_list)
{                                                                         //{{{1
  // set new bit information
  bit_info *new_bit = new bit_info(mAddr, this);
  assert(new_bit != NULL);
  new_bit->mEndAddr      = eaddr;
  new_bit->mStartAddr    = saddr;
  new_bit->mBitName      = bit_name;
  new_bit->mInitVal      = init_val;
  new_bit->mMode[0]      = mode;
  new_bit->mSpp          = spp;
  new_bit->mWritableType = value_type;
  new_bit->mWritableList = writable_list;
  new_bit->mAccessStatus = emNotAccessed;
  new_bit->next          = mBitInfo;

  mBitInfo = new_bit;

  // regist initial value
  unsigned int val = mParent->rd_cb(mAddr);

  if (eaddr-saddr+1 == REG_SIZE) { // bit clear at specified area to write
    val = 0;
  }
  else {
    val &= ~(((1<<(eaddr-saddr+1))-1) <<saddr);
  }

  val |= (init_val<<saddr); // bit set to write specified value

  mParent->wr_cb(mAddr, val);

}                                                                         //}}}1
//
// register access
//
// -----------------------------------------------------------------------------
// Assign the whole of register
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator = (unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {message(emFatalLevel, "Not initialized yet"); return *this;}

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }
  else {
#ifndef ACC_CTRL_ON_DBG_MODE
    mParent->wr_cb(mAddr, data);
#else
    bool backup_fatal   = mMessageLevel["fatal"];
    bool backup_error   = mMessageLevel["error"];
    bool backup_warning = mMessageLevel["warning"];
    bool backup_info    = mMessageLevel["info"];

    mMessageLevel["fatal"]   = false;
    mMessageLevel["error"]   = false;
    mMessageLevel["warning"] = false;
    mMessageLevel["info"]    = false;

    //mParent->wr_cb(mAddr, data);
    for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next)
    {
      it->surpress_message(false);

      if (it->mEndAddr == 31 && it->mStartAddr == 0){
        this->operator[](it->mBitName.c_str()) = data;
      }
      else{
        this->operator[](it->mBitName.c_str()) = (data>>it->mStartAddr)&((1<<(it->mEndAddr-it->mStartAddr+1))-1);
      }
      it->surpress_message(true);
    }

    mMessageLevel["fatal"]   = backup_fatal;
    mMessageLevel["error"]   = backup_error;
    mMessageLevel["warning"] = backup_warning;
    mMessageLevel["info"]    = backup_info;
#endif // ACC_CTRL_ON_DBG_MODE
  }
  return *this;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return the whole of register
// -----------------------------------------------------------------------------
vpcl::re_register::operator unsigned int ()
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return 0;
  }

  return mParent->rd_cb(mAddr);
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Assign the whole of register with access limitation
// -----------------------------------------------------------------------------
bool vpcl::re_register::write(unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return false;
  }
  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return false;
  }

  if ( (~mEntried & data) != 0) {
    message(emWarningLevel, "Cannot write 1 to reserved bit.");
    data &= mEntried;
  }

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next)
  {
    it->surpress_message(false);

    if (it->mEndAddr == 31 && it->mStartAddr == 0){
      this->operator[](it->mBitName.c_str()) = data;
    }
    else{
      this->operator[](it->mBitName.c_str()) = (data>>it->mStartAddr)&((1<<(it->mEndAddr-it->mStartAddr+1))-1);
    }
    it->surpress_message(true);
  }

  return true;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return the whole of register with access limitation
// -----------------------------------------------------------------------------
unsigned int vpcl::re_register::read()
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return 0;
  }

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    unsigned int chk_num = (it->mMode[mAccessNum] == NOT_ENTRY)? 0 : mAccessNum;

    switch (it->mMode[chk_num]) {
      case emAccW:
      case emAccW0:
      case emAccW1:
      case emAccW0S1:
      case emAccW1S0:   // phase 2 added
      case emAccW1T: // added 2015.Feb.11
      case emAccW0T: 
      case emAccWS0: 
      case emAccWS1: 
        message(emWarningLevel, "%s forbids to read", it->mBitName.c_str());
        break;

      case emAccWBR:
        if ((it->mAccessStatus != emWrittenStatus) && (it->mAccessStatus != emWritten0Status) && (it->mAccessStatus != emWritten1Status)) {
          message(emErrorLevel, "Write action is required before reading %s", it->mBitName.c_str());
        }
        break;

      case emAccW0BR:
      case emAccW1BR:
      case emAccW0S1BR:
      case emAccW1S0BR:
        if (((it->mMode[chk_num] == emAccW0BR)||(it->mMode[chk_num] == emAccW0S1BR))&&(it->mAccessStatus != emWritten0Status)) {
          message(emErrorLevel, "Write 0 action is required before reading %s", it->mBitName.c_str());
        } else if (((it->mMode[chk_num] == emAccW1BR)||(it->mMode[chk_num] == emAccW1S0BR))&&(it->mAccessStatus != emWritten1Status)) {
          message(emErrorLevel, "Write 1 action is required before reading %s", it->mBitName.c_str());
        } else {
          it->mAccessStatus = emReadStatus;
        }
        break;

      case emAccD:
        if (mParent->ChkAccess(false,"",mRegName,it->mBitName)) {
          it->mAccessStatus = emReadStatus;
        }
        else {
          message(emWarningLevel, "%s is not allowed to access this time", it->mBitName.c_str());
        }
        break;
      
      default: // emAccR, emAccRW, emAccRBW0, emAccRBW1, emAccRW0, emAccRW1
               // emAccRW1T, emAccRW0T, emAccRWS0, emAccRWS1, emAccRW0S1
        it->mAccessStatus = emReadStatus;
        break;
    }
  }

  unsigned int pre_update_val = mParent->rd_cb(mAddr);

  bool backup_fatal   = mMessageLevel["fatal"];
  bool backup_error   = mMessageLevel["error"];
  bool backup_warning = mMessageLevel["warning"];
  bool backup_info    = mMessageLevel["info"];

  mMessageLevel["fatal"]   = false;
  mMessageLevel["error"]   = false;
  mMessageLevel["warning"] = false;
  mMessageLevel["info"]    = false;

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    unsigned int chk_num = (it->mMode[mAccessNum] == NOT_ENTRY)? 0 : mAccessNum;

    switch (it->mMode[chk_num]) {
      case emAccRS0:
      case emAccRS0W:
      case emAccRS0W0:
      case emAccRS0W1:
        this->operator[](it->mBitName.c_str()) = 0;
        break;

      case emAccRS1:
      case emAccRS1W:
      case emAccRS1W0:
      case emAccRS1W1:
        this->operator[](it->mBitName.c_str()) = 1;
        break;
    }
  }
  mMessageLevel["fatal"]   = backup_fatal;
  mMessageLevel["error"]   = backup_error;
  mMessageLevel["warning"] = backup_warning;
  mMessageLevel["info"]    = backup_info;

  return pre_update_val;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Assign the whole of register with logic and
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator &= (unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
   return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  return this->operator=(data&mParent->rd_cb(mAddr));
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Assign the whole of register with logic or
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator |= (unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  return this->operator=(data|mParent->rd_cb(mAddr));
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Increment
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator ++ (int dummy)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  mParent->wr_cb(mAddr, mParent->rd_cb(mAddr)+1);
  return *this;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Decrement
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator -- (int dummy)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  mParent->wr_cb(mAddr, mParent->rd_cb(mAddr)-1);
  return *this;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// N-increment
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator += (unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  mParent->wr_cb(mAddr, mParent->rd_cb(mAddr)+data);
  return *this;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// N-decrement
// -----------------------------------------------------------------------------
vpcl::re_register &vpcl::re_register::operator -= (unsigned int data)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return *this;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return *this;
  }

  mParent->wr_cb(mAddr, mParent->rd_cb(mAddr)-data);
  return *this;
}                                                                         //}}}1

//
// bit access
//
// -----------------------------------------------------------------------------
// Return specified bit by character
// -----------------------------------------------------------------------------
vpcl::bit_info &vpcl::re_register::operator[](const char *bit_name)
{                                                                         //{{{1
  // cannot avoid Sementation fault because this method returns bit_info
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    bit_info *null_bit = new bit_info (0x0, NULL);
    return *null_bit;
  }

  if (bit_name == NULL) {
    message(emErrorLevel, "Cannot allocate memory at %d (%s)", __LINE__, __FILE__);
    return *mBitInfo;
  }

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    if (it->mBitName == bit_name) {
      return *it;
    }
  }

  message(emErrorLevel, "%s bit is not specified yet", bit_name);

  return *mBitInfo;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return specified bit by single bit number
// -----------------------------------------------------------------------------
unsigned int vpcl::re_register::operator[](unsigned int addr)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return 0;
  }

  return (mParent->rd_cb(mAddr)>>addr)&1;
}                                                                         //}}}1

//
// multi master
//
// -----------------------------------------------------------------------------
// Specify access number
// -----------------------------------------------------------------------------
void vpcl::re_register::access_owner(unsigned int num)
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return;
  }

  if (mIsReset == true) {
    message(emWarningLevel, "Cannot update register during reset period");
    return;
  }

  mAccessNum = num;
  bool entried_flag = false;

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    if (it->mMode[num] == NOT_ENTRY) {
      it->access_owner(0);
    }
    else {
      it->access_owner(num);
      entried_flag = true;
    }
  }

  if (entried_flag == false) {
    message(emErrorLevel, "%dth owner is not entired yet", num);
  }
}                                                                         //}}}1

//
// other
//
// -----------------------------------------------------------------------------
// Initialize register value
// -----------------------------------------------------------------------------
void vpcl::re_register::reset()
{                                                                         //{{{1
  if (mInitialized == false) {
    message(emFatalLevel, "Not initialized yet");
    return;
  }

  unsigned int val = 0;

  for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
    val |= (it->mInitVal<<it->mStartAddr);
  }

  mParent->wr_cb(mAddr, val);
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return register name
// -----------------------------------------------------------------------------
std::string vpcl::re_register::name()
{                                                                         //{{{1
  return mRegName;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Return address
// -----------------------------------------------------------------------------
unsigned int vpcl::re_register::addr()
{                                                                         //{{{1
  return mAddr;
}                                                                         //}}}1

// -----------------------------------------------------------------------------
// Set debug message level 
// -----------------------------------------------------------------------------
void vpcl::re_register::SetMessageLevel(std::map<std::string, bool> DbgMsgLevel)
{
  std::map<std::string, bool>::iterator msgit;
  for (msgit = mMessageLevel.begin(); msgit != mMessageLevel.end(); msgit++) {
    msgit->second = false;
  }
  for (msgit = DbgMsgLevel.begin(); msgit != DbgMsgLevel.end(); msgit++) {
    mMessageLevel[msgit->first] = msgit->second; 
  }
}

// -----------------------------------------------------------------------------
// Set debug message level 
// -----------------------------------------------------------------------------
std::string vpcl::re_register::GetMessageLevel()
{
  std::string ret = "";
  std::map<std::string, bool>::iterator it;
  for (it = mMessageLevel.begin(); it != mMessageLevel.end(); it++) {
    if ( it->second == true ) {
      ret += " " + it->first;
    }
  }
  return ret;
}

// -----------------------------------------------------------------------------
// Update bit initial value
// -----------------------------------------------------------------------------
void vpcl::re_register::UpdateBitInitValue(std::string bit_name, unsigned int value)
{
    bool is_bit_name_correct = false;
    for (bit_info *it=mBitInfo ; it!=NULL ; it=it->next) {
        if (bit_name == it->mBitName) {
            is_bit_name_correct = true;
            unsigned int max_val = 0x0;
            if ((it->mEndAddr - it->mStartAddr) < 31) {
                max_val = (1 << (it->mEndAddr - it->mStartAddr + 1)) - 1;
            } else {
                max_val = 0xFFFFFFFF;
            }
            if (value > max_val) {
                message(emErrorLevel, "Value 0x%X is greater than the max value (0x%X) of %s bit.\n", value, max_val, bit_name.c_str());
            } else {
                it->mInitVal = value;
            }
            break;
        }
    }
    if (!is_bit_name_correct) {
        message(emErrorLevel, "Bit name %s is wrong", bit_name.c_str());
    }
}

// -----------------------------------------------------------------------------
// Update reset status 
// -----------------------------------------------------------------------------
void vpcl::re_register::EnableReset(bool is_active) {
    if (is_active == true){
        reset();
    }
    mIsReset = is_active;
}
// -----------------------------------------------------------------------------
// Initialize message level variable
// -----------------------------------------------------------------------------
void vpcl::re_register::InitMsgLevel(){
  mMessageLevel["fatal"]   = true;
  mMessageLevel["error"]   = true;
  mMessageLevel["warning"] = false;
  mMessageLevel["info"]    = false;
}

// -----------------------------------------------------------------------------
// Error/Warning message
// -----------------------------------------------------------------------------
void vpcl::re_register::_message (eMsgType type, std::string fmt, ...)
{                                                                         //{{{1
  char msg_type [8];
  va_list argptr;

  // message level check
  bool dump = false;
  if ((mMessageLevel["fatal"]   == true)  && type == emFatalLevel  ) { dump = true;}
  if ((mMessageLevel["error"]   == true)  && type == emErrorLevel  ) { dump = true;}
  if ((mMessageLevel["warning"] == true)  && type == emWarningLevel) { dump = true;}
  if ((mMessageLevel["info"]    == true)  && type == emInfoLevel   ) { dump = true;}
  if (dump == false) { return;}

  // fatal error check
  if (fmt == "") {
    message(emErrorLevel, "Cannot allocate memory at %d (%s)", __LINE__, __FILE__);
    return;
  }

  // dump message body
  va_start (argptr,fmt);
  assert(argptr != NULL);

       if (type == emFatalLevel  ) { strcpy (msg_type,"FATAL"  );}
  else if (type == emErrorLevel  ) { strcpy (msg_type,"ERROR"  );}
  else if (type == emWarningLevel) { strcpy (msg_type,"WARNING");}
  else if (type == emInfoLevel   ) { strcpy (msg_type,"INFO"   );}

  #ifdef REGIF_SC_REPORT
  char str[1024];
  unsigned int len;

  if (type == emFatalLevel) {
    len = sprintf (str, "[%s:%d] %s ", mFileName.c_str(), mLineNum, msg_type);
    vsprintf(str+len, fmt.c_str(), argptr);
  }
  else {
    len = sprintf (str, "[%s:%s (%d)] %s ",mModName.c_str(), mRegName.c_str(), mAccessNum, msg_type);
    vsprintf(str+len, fmt.c_str(), argptr);
  }

  if (type == emFatalLevel) {
      SC_REPORT_FATAL(mModName.c_str(), str);
  }
  else if (type == emErrorLevel) {
      SC_REPORT_ERROR(mModName.c_str(), str);
  }
  else if (type == emWarningLevel) {
      SC_REPORT_WARNING(mModName.c_str(), str);
  }
  else if (type == emInfoLevel) {
      SC_REPORT_INFO(mModName.c_str(), str);
  }
  #else //REGIF_SC_REPORT
  if (type == emFatalLevel) {
    printf ("[%s:%d] %s ", mFileName.c_str(), mLineNum, msg_type);
  }
  else {
    printf ("[%s:%s (%d)] %s ",mModName.c_str(), mRegName.c_str(), mAccessNum, msg_type);
  }

  vprintf (fmt.c_str(),argptr);
  printf ("\n");fflush(stdout);
  #endif//REGIF_SC_REPORT

  va_end (argptr);

}                                                                         //}}}1
/// Get file name and line number
/// @return none
void vpcl::re_register::get_fileline(std::string filename, unsigned int line_number) 
{
    mFileName = filename;
    mLineNum  = line_number;
}
// vim600: set foldmethod=marker :
