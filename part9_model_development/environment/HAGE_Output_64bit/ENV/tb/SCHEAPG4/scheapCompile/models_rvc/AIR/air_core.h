// ## ----------------------------------------------------------------------
// ## Copyright(c) 2016 - 2017 Renesas System Design Co., Ltd.
// ## Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// ## RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// ## This program must be used solely for the purpose for which
// ## it was furnished by Renesas Electronics Corporation. No part of this
// ## program may be reproduced or disclosed to others, in any
// ## form, without the prior written permission of Renesas Electronics
// ## Corporation.
// ## ----------------------------------------------------------------------
#ifndef __AIR_CORE_H__
#define __AIR_CORE_H__
#include <string>
#include <iterator>
#include "air_core_regif.h"

class Cair;
/// AIR_CORE class
class Cair_core: public Cair_core_regif
{
public:
    friend class Cair;
    enum eAirCoreConstant {
        emResource0 = 0,
        emResource1 = 1,
        emNotResource = 2
    };

    Cair_core(std::string name, Cair *air);
    ~Cair_core();

    void NotifyInputPortChange(unsigned int index, bool is_active);
    bool GetInputPort(unsigned int index);
    bool WriteRegister(cuint addr, const unsigned char *p_data, cuint size, bool is_debug);
    bool ReadRegister(cuint addr, unsigned char *p_data, cuint size, bool is_debug);
    void ResetRegister(bool is_active);
    std::string regif_handle_command(std::vector<std::string>& args);
    bool strmatch(const char *ptn, const char *str);

private: 
    Cair *pAirWrapper; 

    struct air_mux_table{
        bool  inValue;
        vpcl::bit_info* mux;
        unsigned int resourceID;
        unsigned int outPort;
        bool outValue;

        air_mux_table(bool inValue, vpcl::bit_info* mux, unsigned int id, unsigned int outPort, bool outValue) {
            this->inValue = inValue;
            if(mux != NULL){
                this->mux = mux;
            }
            this->resourceID = id;
            this->outPort = outPort;
            this->outValue = outValue;
        }
    };
    std::vector<air_mux_table> mux_table_int;
    std::vector<air_mux_table> mux_table_dma;

};
#endif //__AIR_CORE_H__
