#include "PY_DummyMasterRvc.h"
#include "PY_DummyPeripheralRvc.h"
#include "PY_HAGE.h"

static void UT_PY_Load (){
    PY_DummyMasterRvc::SetPyExtCmd();
    PY_DummyPeripheralRvc::SetPyExtCmd();
    PY_HAGE::SetPyExtCmd();
}
