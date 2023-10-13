//----------------------------------------------------------------------
//* File: Memory.h
//* Description: Memory model header 
//* Modified date: 
//----------------------------------------------------------------------
#pragma once

#include "systemc.h"
#include "BusSlaveBase.h"
#include "BusSlaveFuncIf.h"
#include <iterator>
#include <vector>
#include <map>
#include <iostream>
#include <string>

class MEMORYRVC : public BusSlaveFuncIf
{
public:
    MEMORYRVC(void);
    ~MEMORYRVC(void);

    SC_HAS_PROCESS(MEMORYRVC);

    //API Python
    void CreateMem(unsigned int m_address, unsigned int m_size);
    void SetData(unsigned int set_addr, unsigned int data);
    void GetData(unsigned int get_addr, unsigned int ex_data);
    void DeleteMem();
    void Judgement(bool judge);
    int GetMemIndex(unsigned int addr);
    //For slave - virtual functions in BusSlaveFuncIf
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, sc_time* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, sc_time* t, bool debug);

private:
    struct Memory_controll {
        unsigned int BaseAddress;
        unsigned int MemSize;
    };
    std::map<unsigned int, unsigned char> mMemory;
    std::vector<Memory_controll*> mMemory_Controller;
};
