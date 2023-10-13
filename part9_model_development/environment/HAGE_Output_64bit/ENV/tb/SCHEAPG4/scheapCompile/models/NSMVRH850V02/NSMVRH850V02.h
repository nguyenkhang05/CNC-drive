/*************************************************************************
 *
 *  NSMVRH850V02.h
 *
 * Copyright(c) 2016 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef NSMVRH850V02_H
#define NSMVRH850V02_H

class NSMVG4SSV01;
class PFRH850;

class NSMVRH850V02 : public sc_module
{

  // type of peripheral connection
  enum {
    PERIPHERAL_NONE   = 0,      // no peripheral
    PERIPHERAL_RH850   = 1,     // RH850
    PERIPHERAL_SMPILS = 2,      // SMPILS
    PERIPHERAL_DEFAULT = 0x8000 // default
  };

 public:
  NSMVRH850V02(sc_module_name moduleName, const char *configFile );
  
  ~NSMVRH850V02(void);
  
  NSMVG4SSV01                *G4SS; // pointer of G4SS
 private:
  void read_config_file( const char *filename );
  unsigned int get_cfg_PERIPHERAL(char *param_str, const char *mod_name, const char *func_name);
  
  unsigned int mPltfrmSize; //platform size
  unsigned int mPeripheral; //type of peripheral connection

  struct {
    void (*fpltfrm)(NSMVG4SSV01*, PFRH850*, const char*); // funciton for module which is connected after RH850 peripherals
    void (*fpltfrmFC)(NSMVG4SSV01*, const char*);         // function for module which is connected before RH850 peripherals
    void (*fpltfrmDelete)();                               // delete function
    unsigned int peripheral;                               // peripheral connection type
  } funcPltfrm[5]; // platform connection function pointer

}; // end of NSMVRH850V02 definition

#endif /* NSMVRH850V02_H */
