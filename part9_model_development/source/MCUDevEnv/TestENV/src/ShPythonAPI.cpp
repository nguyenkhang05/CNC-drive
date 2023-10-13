#include "BV_TestENV.h"
#include "ShPythonAPI.h"
#include "CmErrMsg.h" // for WinMsg
#ifndef RVC_GTEST
#include "UT_Python_Load.h"
#endif // RVC_GTEST
#include <stdio.h>  /* defines FILENAME_MAX */

using namespace CmErrMsg; // for WinMsg
extern void UT_PY_Load();
#define PYTHON_HOME "SCHEAP_PYTHON_HOME"

/// <summary>python start function</summary>
/// <param name="argc">number of argument</param>
/// <param name="argv">pointer of argument</param>
void ShPythonAPI::StartPy(int argc, char* argv[])
{

    // Show Python home
    char *py_home = getenv(PYTHON_HOME);
    if (py_home) {
        Py_SetPythonHome(py_home);
    }

    Py_Initialize();

    LoadShCommandDefPy();

    if (glb_python_scr_file) {
        char command_string[1024];
        sprintf(command_string,
            "execfile(\"%s\")",
            glb_python_scr_file);
        PyRun_SimpleString(command_string);
    }
    else {
        Py_Main(1, argv);
    }

    Py_Finalize();
}


/// <summary>python destructor</summary>
void ShPythonAPI::DestructorPy(void)
{
}

/// <summary>load python command</summary>
void ShPythonAPI::LoadShCommandDefPy()
{
    SetPyExtCmd(); //main
    // Model UT define
#ifndef RVC_GTEST
    UT_PY_Load();
#endif // RVC_GTEST
    // -----------------
    static const char*  CODES_Template = "import SCHEAP\n";
    char  CODES[16384];

    sprintf(CODES, CODES_Template);

    PyRun_SimpleString(CODES);
}
