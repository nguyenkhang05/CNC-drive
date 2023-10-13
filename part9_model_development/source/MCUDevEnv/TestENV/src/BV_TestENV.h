#ifndef __BV_TEST_ENV_H__
#define __BV_TEST_ENV_H__

#include "systemc.h"
#include "sstream"
//---- global variable
extern int           scstopDoneFlag;
extern sc_dt::uint64 glb_freq;
extern double        glb_resolution_period;
extern sc_time_unit  glb_resolution_unit;
extern int           glb_cycle_number;
extern char          *glb_python_scr_file;

extern void SetPyExtCmd(void);

void BV_InitTest(int argc, char **argv);
void BV_DeInitTest(void);

#endif // !__BV_TEST_ENV_H__
