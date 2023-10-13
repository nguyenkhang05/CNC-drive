//----------------------------------------------------------------------
//* File: Memory.cpp
//* Description: Memory model
//* Modified date: 
//----------------------------------------------------------------------

#include "DummyPeripheralRvc.h"
#include "MemoryRVC.h"
#include "BusSlaveFuncIf.h"

MEMORYRVC::MEMORYRVC(void) :
    BusSlaveFuncIf()
{
}
MEMORYRVC::~MEMORYRVC(void) {
    DeleteMem();
}

void MEMORYRVC::CreateMem(unsigned int m_address, unsigned int m_size)
{
    Memory_controll* memory_controller = new Memory_controll;
    //Store Base address, memsize
    memory_controller->BaseAddress = m_address;
    memory_controller->MemSize = m_size;
    //Create memory. Init value=0x00
    for (int i = 0; i < m_size; i++) {
        mMemory.insert(std::pair<unsigned int, unsigned char>(m_address + i, 0x00));
    }
    mMemory_Controller.push_back(memory_controller);
    printf("Create a 0x%X size memory at 0x%08X\n", m_size, m_address);
}

void MEMORYRVC::SetData(unsigned int set_addr, unsigned int data)
{
    //Using Big Endian
    unsigned char buffer[4];
    buffer[0] = (data >> 24) & 0xFF;
    buffer[1] = (data >> 16) & 0xFF;
    buffer[2] = (data >> 8) & 0xFF;
    buffer[3] = data & 0xFF;

    unsigned int data_length = 4;
    printf("Set 0x%08X: ", set_addr);
    for (int i = 0; i < data_length; i++) {
        if (mMemory.find(set_addr + i) != mMemory.end()) {
            mMemory[set_addr + i] = buffer[i];
            printf("%x", mMemory[set_addr + i]);
        }
        else {
            printf("Set address out of range\n");
            break;
        }
    }
    printf("\n");

}

void MEMORYRVC::GetData(unsigned int get_addr, unsigned int ex_data)
{
    unsigned char buffer[4];
    buffer[0] = (ex_data >> 24) & 0xFF;
    buffer[1] = (ex_data >> 16) & 0xFF;
    buffer[2] = (ex_data >> 8) & 0xFF;
    buffer[3] = ex_data & 0xFF;
    unsigned int data_length = 4;
    printf("Get 0x%08X: ", get_addr);
    for (int i = 0; i < data_length; i++) {
        if (mMemory.find(get_addr + i) != mMemory.end()) {
            if (mMemory[get_addr + i] != buffer[i]) {
                Judgement(0);
            }
            else
            {
                printf("%X", mMemory[get_addr + i]);
            }
        }
        else {
            printf("Get address out of range.\n");
            Judgement(0);
        }
    }
    printf("\n");
}

void MEMORYRVC::DeleteMem() {

    for (int i = 0; i < mMemory_Controller.size(); i++) {
        delete mMemory_Controller[i];
    }
    mMemory.clear();
    mMemory_Controller.clear();
    if (mMemory.empty() && mMemory_Controller.empty()) printf("Memory is clear\n");
}

//Return Memory index in Vector mMemory
int MEMORYRVC::GetMemIndex(unsigned int addr) {
    int mem_index = -1;
    for (int i = 0; i < mMemory_Controller.size(); i++) {
        if ((addr >= mMemory_Controller[i]->BaseAddress) && (addr <= mMemory_Controller[i]->BaseAddress + mMemory_Controller[i]->MemSize - 1))
        {
            mem_index = i;
        }
    }
    return mem_index;
}

void MEMORYRVC::Judgement(bool judge)
{
    if (judge == 0) {
        printf("--------------------------------\n");
        printf("-          TM is FAIL          -\n");
        printf("--------------------------------\n");
        exit(1);
    }
    else if (judge == 1) {
        printf("--------------------------------\n");
        printf("-          TM is PASS          -\n");
        printf("--------------------------------\n");
    }
}

void MEMORYRVC::read(unsigned int offsetAddress, TlmBasicPayload& trans, sc_time* t, bool debug)
{
    unsigned int readAddress;
    readAddress = trans.get_address();
    int mem_index = GetMemIndex(readAddress);
    unsigned int data_len = trans.get_data_length();
    unsigned max_index_buffer = 3;
    if (mem_index < 0) {
        // Write fail becasue readAddress out range of mMemory
        printf("Read Data fail, becasue readAddress out range of mMemory\n");
    }
    else {
        if ((mMemory_Controller[mem_index]->BaseAddress <= (readAddress)) && ((readAddress + data_len) <= mMemory_Controller[mem_index]->BaseAddress + mMemory_Controller[mem_index]->MemSize))
        {
            for (int i = 0; i < data_len; i++) {
                memcpy(trans.get_data_ptr() + i, &mMemory[readAddress + i], 1);
            }
        }
    }
}

void MEMORYRVC::write(unsigned int offsetAddress, TlmBasicPayload& trans, sc_time* t, bool debug)

{
    unsigned int writeAddress;
    writeAddress = trans.get_address();
    int mem_index = GetMemIndex(writeAddress);
    unsigned int data_len = trans.get_data_length();
    unsigned char* data_ptr = trans.get_data_ptr();
    if (mem_index < 0) {
        // Write fail becasue writeAddress out range of mMemory
        printf("Write Data fail, becasue writeAddress out range of mMemory\n");
    }
    else {
        if ((mMemory_Controller[mem_index]->BaseAddress <= (writeAddress)) && ((writeAddress + data_len) <= mMemory_Controller[mem_index]->BaseAddress + mMemory_Controller[mem_index]->MemSize))
        {
            for (int i = 0; i < data_len; i++) {
                memcpy(&mMemory[writeAddress + i], trans.get_data_ptr() + i, 1);
            }
        }
    }
}