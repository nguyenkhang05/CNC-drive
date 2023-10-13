#include "BV_TestENV.h"
#include <stdlib.h>  // for strtol, strtoul, strtod
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <signal.h>
#include <strstream>
#include <string>
#include <errno.h>   // for errno
#include "CmErrMsg.h"
#include <time.h>
#include <Python.h>
#include "systemc.h"
#ifdef TEMPORAL_DECOUPLING
#include <tlm_utils/tlm_quantumkeeper.h>
#endif //TEMPORAL_DECOUPLING
#include "CmErrMsg.h" // for WinMsg
#include "ShPythonAPI.h"
using namespace CmErrMsg;

#define FREQ_DEFAULT 10000000 // default value of frequency

//---- global variable
int           scstopDoneFlag;
sc_dt::uint64 glb_freq = FREQ_DEFAULT;      // global simulation frequency
double        glb_clock_period;
sc_time_unit  glb_clock_time_unit;          // move from top.cpp
double        glb_resolution_period;
sc_time_unit  glb_resolution_unit;          // move from top.cpp
char          *glb_python_scr_file;         // python script file 
sc_signal<sc_dt::uint64, sc_core::SC_UNCHECKED_WRITERS> *glb_pe_clk_sig;   // global clock signal
ofstream      bus_monitor_ofs;              // ofstream for bus monitor
using namespace CmErrMsg;                   // for WinMsg
                                       //for systemc2.2 deprecated feature.
double org_sc_simulation_time(void) 
{
    sc_time t = sc_time_stamp();
    double unit = sc_time(1, glb_clock_time_unit) / sc_get_time_resolution();
    return(t.value() / unit);
}

#ifdef RH850
#include "NSMVRH850LITE.h"
#include "NSMVG4SSLITE.h"
#else
extern void UT_Test_Init(const char *config_file);
extern void UT_Test_DeInit();
#endif

void BV_InitTest(int argc, char **argv)
{
    sc_report_handler::set_actions(SC_ERROR, SC_DEFAULT_WARNING_ACTIONS);
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    const char  *config_file = 0;
    int          cycle_number = 10;
    char         seps[] = " ";
    char         config_word[512];
    char         config_seps[] = " ,=();\t\n\r";
    char        *config_token;
    double       clock_period = 10.0;
    sc_time_unit clock_time_unit = SC_NS;
    std::string  time_resolution = "";

    ShPythonAPI mShPythonAPI;
    glb_python_scr_file = 0;
    glb_pe_clk_sig = new sc_signal<sc_dt::uint64, sc_core::SC_UNCHECKED_WRITERS>("glb_pe_clk_sig");

    if (argc > 1) {
        for (int count = 1; count < argc; count++) {
            if (strcmp(argv[count], "-config") == 0) {
                config_file = argv[count + 1];
            }
            else if (strcmp(argv[count], "-py_scr") == 0) {
                glb_python_scr_file = argv[count + 1];
            }
            else if (strcmp(argv[count], "-sctime") == 0) {
                time_resolution = argv[count + 1];
            }
            else if (strcmp(argv[count], "-help") == 0) {
                cout << "Usage : " << argv[0]
                    << " -config config_file"
                    << " -py_scr python_script_file"
                    << " [-v]"
                    << " [-help]" << endl;
                exit(1);
            }
        }
    }
    else {
        cout << "Running GTEST " << endl;
    }
#ifndef RVC_GTEST
    if (!config_file) {
        cout << "Error: cannot open config file\n"
            << endl;
        exit(1);
    }
    CmErrMsgInitSet(config_file); // for WinMsg

    ifstream configFile_(config_file);

    if (!configFile_) {
        // for WinMsg
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Error: cannot open config file %s\n",
            config_file);
        printErrorMessage("main", "main", error_message_buf);
        exit(1);
    }

    if (glb_python_scr_file) {
        ifstream scr_file(glb_python_scr_file);
        if (!scr_file) {
            char error_message_buf[1024];
            sprintf(error_message_buf,
                "Error: cannot open Python script file %s\n",
                glb_python_scr_file);
            printErrorMessage("main", "main", error_message_buf);
            exit(1);
        }
    }
#endif // RVC_GTEST
    scstopDoneFlag = 0;
    glb_clock_period = 10.0;
    glb_clock_time_unit = SC_NS;
    // read the config file (deleted)

    glb_resolution_period = 1;
    glb_resolution_unit = SC_PS;
    if (time_resolution == "") {
        glb_resolution_unit = SC_PS;
    }
    else if (time_resolution == "SC_FS") {
        glb_resolution_unit = SC_FS;
    }
    else if (time_resolution == "SC_PS") {
        glb_resolution_unit = SC_PS;
    }
    else if (time_resolution == "SC_NS") {
        glb_resolution_unit = SC_NS;
    }
    else if (time_resolution == "SC_US") {
        glb_resolution_unit = SC_US;
    }
    else if (time_resolution == "SC_MS") {
        glb_resolution_unit = SC_MS;
    }
    else if (time_resolution == "SC_SEC") {
        glb_resolution_unit = SC_SEC;
    }
    else {
        glb_resolution_unit = SC_PS;
    }
    sc_set_time_resolution(glb_resolution_period, glb_resolution_unit);


#ifdef RH850
    NSMVRH850LITE rh850("RH850", config_file);
#else
    /* Add <Model>_Test_Init() functions */
    UT_Test_Init(config_file);
#endif


    /* systemC start must be in the end */

    // Call Python start
    if (!glb_python_scr_file) {
        CmErrMsgSetToFile(false);
        sc_start();
    }
    else {
        mShPythonAPI.StartPy(argc, argv);
        mShPythonAPI.DestructorPy();
    }
}

void BV_DeInitTest(void)
{
#ifndef RH850
    /* Add <Model>_Test_DeInit() functions */
    UT_Test_DeInit();
#endif
    /* systemC stop must be in the end */
    sc_stop();
    delete glb_pe_clk_sig;
}

/// <summary>open the bus monitor file</summary>
/// <param name="fName">file name</param>
void BusMonitorFileOpn(char *fName)
{
    std::ostringstream msg;

    if (bus_monitor_ofs.is_open()) { //The file is already opened
        msg << "Could not open file[" << fName << "] because another file is already opened for bus monitor." << endl;
        printWarningMessage("BusMonitor",
            "FileOpnPy",
            msg.str().c_str());
        return;
    }

    bus_monitor_ofs.open(fName);

    if (!bus_monitor_ofs) { // file open error
        msg << "Could not open file[" << fName << "] for bus monitor.\n                     Simulation continues to run without bus monitor." << endl;
        printWarningMessage("BusMonitor",
            "FileOpnPy",
            msg.str().c_str());
        return;
    }
    bus_monitor_ofs << ",,,,,Generic Payload,,,,,,";
    bus_monitor_ofs << "G3M Extension Payload,,,,,,,,,,";
    bus_monitor_ofs << "AHB Extension Payload,,,,,,";
    bus_monitor_ofs << "APB Extension Payload,,";
    bus_monitor_ofs << "AXI Extension Payload,,,,,,,,";
    bus_monitor_ofs << "VCI Extension Payload";
    bus_monitor_ofs << endl;
    bus_monitor_ofs << "Sim Time,Bus,TransType,Direction,Local Time,Payload,R/W,Address,Length,Data,Status,";
    bus_monitor_ofs << "Master Name,DMA,TCID,VMID,PEID,SPID,UM,VM,SPCLD,Secure,";
    bus_monitor_ofs << "Burst,Lock,Cachable,Bufferable,Privilege,Data/OP,";
    bus_monitor_ofs << "Lock,Exclusion,";
    bus_monitor_ofs << "Burst,Lock,Cachable,Bufferable,Secure,TransId,BitOpType,BitOpPosi,";
    bus_monitor_ofs << "ReqType";
    bus_monitor_ofs << endl;
}

/// <summary>close bus monitor file</summary>
void BusMonitorFileCls(void)
{
    std::ostringstream msg;

    if (!(bus_monitor_ofs.is_open())) { //The file is already closed
        msg << "Could not close the bus monitor file because the file is already closed." << endl;
        printWarningMessage("PY_BusMonitor",
            "FileClsPy",
            msg.str().c_str());
        return;
    }
    bus_monitor_ofs.close();
}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* sc_startPy(PyObject* self, PyObject* args)
{
    unsigned int cycle_i;

    PyArg_ParseTuple(args, "i", &cycle_i);

    if (glb_freq) {
        sc_start(cycle_i*(1 / (double)glb_freq), SC_SEC);
    }
    else {
        sc_start(SC_ZERO_TIME);
    }
    return Py_BuildValue("");
}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* sc_stopPy(PyObject* self, PyObject* args)
{
    sc_stop();
    return Py_BuildValue("");
}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* setFreqPy(PyObject* self, PyObject* args)
{
    unsigned long long freq;
    char   *unitStr;

    PyArg_ParseTuple(args, "Ks", &freq, &unitStr);

    glb_freq = freq;

    if (strcmp(unitStr, "GHz") == 0) {
        glb_freq = glb_freq * (sc_dt::uint64)(1.0e+9);
    }
    else if (strcmp(unitStr, "MHz") == 0) {
        glb_freq = glb_freq * (sc_dt::uint64)(1.0e+6);
    }
    else if (strcmp(unitStr, "KHz") == 0) {
        glb_freq = glb_freq * (sc_dt::uint64)(1.0e+3);
    }
    else if (strcmp(unitStr, "Hz") == 0) {
    }
    else {
        // token error
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Unexpected token was specified [%s] as 2nd parameter of setFreq. Treat default value[Hz].",
            unitStr);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("");
    }

    *glb_pe_clk_sig = glb_freq;

    return Py_BuildValue("");
}

#ifndef BEFORE_SIGNAL_WRITE_BY_PYTHON

/// <summary>Write port or signal value</summary>
/// <param name="value">write value</param>
/// <param name="args">pointer of python object</param>
/// <param name="format">format of PyArg_ParseTuple</param>
/// <param name="str">return strings</param>
template<typename VT, typename PT>
void writePortSignalValue(VT& value, PT obj, PyObject* args,
    char* format, std::ostringstream& str) 
{
    char *name;
    char *data_type;
    char *dummy;

    PyArg_ParseTuple(args, format, &name, &data_type, &value, &dummy);
    if (!(obj = dynamic_cast<PT>(sc_find_object(name)))) {
        char message[1024];
        sprintf(message,
            "There is not the port or signal named %s.\n",
            name);
        printWarningMessage("main", "main", message);
        return;
    }
    obj->write(value);
    if (typeid(VT) == typeid(char) ||
        typeid(VT) == typeid(unsigned char)) {
        str << "Wrote " << (int)value << " to " << name << endl;
    }
    else {
        str << "Wrote " << value << " to " << name << endl;
    }
}

/// <summary>Read port or signal value</summary>
/// <param name="value">read value</param>
/// <param name="args">pointer of python object</param>
/// <param name="format">format of PyArg_ParseTuple</param>
/// <param name="str">return strings</param>
template<typename VT, typename PT>
void readPortSignalValue(VT& value, PT obj, PyObject* args,
    char* format, std::ostringstream& str) 
{
    char *name;
    char *data_type;
    char *dummy;

    PyArg_ParseTuple(args, format, &name, &data_type, &dummy);
    if (!(obj = dynamic_cast<PT>(sc_find_object(name)))) {
        char message[1024];
        sprintf(message,
            "There is not the port or signal named %s.\n",
            name);
        printWarningMessage("main", "main", message);
        return;
    }
    value = obj->read();
    if (typeid(VT) == typeid(char) ||
        typeid(VT) == typeid(unsigned char)) {
        str << "Read " << (int)value << " from " << name << endl;
    }
    else {
        str << "Read " << value << " from " << name << endl;
    }
}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* writeSignalValuePy(PyObject* self, PyObject* args)
{
    char *port_name;
    char *data_type;
    char *write_policy;
    unsigned long long dummy;
    std::ostringstream str;

    if (!PyArg_ParseTuple(args, "ssKs", &port_name, &data_type, &dummy, &write_policy)) {
        if (!PyArg_ParseTuple(args, "ssK", &port_name, &data_type, &dummy)) {
            //parse error
            char error_message_buf[1024];
            sprintf(error_message_buf,
                "Parse error of writeSignalValue.\n",
                port_name);
            printWarningMessage("main", "main", error_message_buf);
            return Py_BuildValue("s", error_message_buf);
        }
        else {
            write_policy = new char;
            write_policy[0] = '\0';
        }
    }

    if (!strcmp(data_type, "bool")) {
        bool value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<bool, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssbs", str);
        }
        else {
            sc_signal<bool>* sig = 0;
            writePortSignalValue(value, sig, args, "ssb", str);
        }
    }
    else if (!strcmp(data_type, "char")) {
        char value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<char, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "sscs", str);
        }
        else {
            sc_signal<char>* sig = 0;
            writePortSignalValue(value, sig, args, "ssc", str);
        }
    }
    else if (!strcmp(data_type, "unsigned char")) {
        unsigned char value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned char, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssBs", str);
        }
        else {
            sc_signal<unsigned char>* sig = 0;
            writePortSignalValue(value, sig, args, "ssB", str);
        }
    }
    else if (!strcmp(data_type, "short")) {
        short value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<short, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "sshs", str);
        }
        else {
            sc_signal<short>* sig = 0;
            writePortSignalValue(value, sig, args, "ssh", str);
        }
    }
    else if (!strcmp(data_type, "unsigned short")) {
        unsigned short value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned short, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssHs", str);
        }
        else {
            sc_signal<unsigned short>* sig = 0;
            writePortSignalValue(value, sig, args, "ssH", str);
        }
    }
    else if (!(strcmp(data_type, "int") &&
        strcmp(data_type, "long"))) {
        int value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<int, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssis", str);
        }
        else {
            sc_signal<int>* sig = 0;
            writePortSignalValue(value, sig, args, "ssi", str);
        }
    }
    else if (!(strcmp(data_type, "unsigned int") &&
        strcmp(data_type, "unsigned long"))) {
        unsigned int value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned int, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssIs", str);
        }
        else {
            sc_signal<unsigned int>* sig = 0;
            writePortSignalValue(value, sig, args, "ssI", str);
        }
    }
    else if (!strcmp(data_type, "long long")) {
        long long value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<long long, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssLs", str);
        }
        else {
            sc_signal<long long>* sig = 0;
            writePortSignalValue(value, sig, args, "ssL", str);
        }
    }
    else if (!strcmp(data_type, "unsigned long long")) {
        unsigned long long value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned long long, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssKs", str);
        }
        else {
            sc_signal<unsigned long long>* sig = 0;
            writePortSignalValue(value, sig, args, "sssK", str);
        }
    }
    else if (!strcmp(data_type, "float")) {
        float value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<float, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssfs", str);
        }
        else {
            sc_signal<float>* sig = 0;
            writePortSignalValue(value, sig, args, "ssf", str);
        }
    }
    else if (!strcmp(data_type, "double")) {
        double value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<double, SC_UNCHECKED_WRITERS>* sig = 0;
            writePortSignalValue(value, sig, args, "ssds", str);
        }
        else {
            sc_signal<double>* sig = 0;
            writePortSignalValue(value, sig, args, "ssd", str);
        }
    }
    else {
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Unexpected token was specified [%s] as 2nd parameter of writeSignalValue.",
            data_type);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    cout << str.str().c_str() << endl;
    return Py_BuildValue("s", str.str().c_str());

}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* readSignalValuePy(PyObject* self, PyObject* args)
{
    char *port_name;
    char *data_type;
    char *write_policy;
    std::ostringstream str;

    if (!PyArg_ParseTuple(args, "sss", &port_name, &data_type, &write_policy)) {
        if (!PyArg_ParseTuple(args, "ss", &port_name, &data_type)) {
            //parse error
            char error_message_buf[1024];
            sprintf(error_message_buf,
                "Parse error of readSignalValue.\n",
                port_name);
            printWarningMessage("main", "main", error_message_buf);
            return Py_BuildValue("s", error_message_buf);
        }
        else {
            write_policy = new char;
            write_policy[0] = '\0';
        }
    }

    if (!strcmp(data_type, "bool")) {
        bool value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<bool, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<bool>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "char")) {
        char value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<char, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<char>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "unsigned char")) {
        unsigned char value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned char, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<unsigned char>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "short")) {
        short value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<short, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<short>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "unsigned short")) {
        unsigned short value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned short, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<unsigned short>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!(strcmp(data_type, "int") &&
        strcmp(data_type, "long"))) {
        int value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<int, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<int>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!(strcmp(data_type, "unsigned int") &&
        strcmp(data_type, "unsigned long"))) {
        unsigned int value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned int, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<unsigned int>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "long long")) {
        long long value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<long long, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<long long>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "unsigned long long")) {
        unsigned long long value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<unsigned long long, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<unsigned long long>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "float")) {
        float value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<float, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<float>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else if (!strcmp(data_type, "double")) {
        double value;
        if (!strcmp(write_policy, "SC_UNCHECKED_WRITERS")) {
            sc_signal<double, SC_UNCHECKED_WRITERS>* sig = 0;
            readPortSignalValue(value, sig, args, "sss", str);
        }
        else {
            sc_signal<double>* sig = 0;
            readPortSignalValue(value, sig, args, "ss", str);
        }
    }
    else {
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Unexpected token was specified [%s] as 2nd parameter of readSignalValue.",
            data_type);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    cout << str.str().c_str() << endl;
    return Py_BuildValue("s", str.str().c_str());

}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* writePortValuePy(PyObject* self, PyObject* args)
{
    char *port_name;
    char *data_type;
    unsigned long long dummy;
    std::ostringstream str;

    if (!PyArg_ParseTuple(args, "ssK", &port_name, &data_type, &dummy)) {
        //parse error
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Parse error of writePortValue.\n",
            port_name);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    if (!strcmp(data_type, "bool")) {
        bool value;
        sc_out<bool>* port = 0;
        writePortSignalValue(value, port, args, "ssb", str);
    }
    else if (!strcmp(data_type, "char")) {
        char value;
        sc_out<char>* port = 0;
        writePortSignalValue(value, port, args, "ssc", str);
    }
    else if (!strcmp(data_type, "unsigned char")) {
        unsigned char value;
        sc_out<unsigned char>* port = 0;
        writePortSignalValue(value, port, args, "ssB", str);
    }
    else if (!strcmp(data_type, "short")) {
        short value;
        sc_out<short>* port = 0;
        writePortSignalValue(value, port, args, "ssh", str);
    }
    else if (!strcmp(data_type, "unsigned short")) {
        unsigned short value;
        sc_out<unsigned short>* port = 0;
        writePortSignalValue(value, port, args, "ssH", str);
    }
    else if (!(strcmp(data_type, "int") &&
        strcmp(data_type, "long"))) {
        int value;
        sc_out<int>* port = 0;
        writePortSignalValue(value, port, args, "ssi", str);
    }
    else if (!(strcmp(data_type, "unsigned int") &&
        strcmp(data_type, "unsigned long"))) {
        unsigned int value;
        sc_out<unsigned int>* port = 0;
        writePortSignalValue(value, port, args, "ssI", str);
    }
    else if (!strcmp(data_type, "long long")) {
        long long value;
        sc_out<long long>* port = 0;
        writePortSignalValue(value, port, args, "ssL", str);
    }
    else if (!strcmp(data_type, "unsigned long long")) {
        unsigned long long value;
        sc_out<unsigned long long>* port = 0;
        writePortSignalValue(value, port, args, "ssK", str);
    }
    else if (!strcmp(data_type, "float")) {
        float value;
        sc_out<float>* port = 0;
        writePortSignalValue(value, port, args, "ssf", str);
    }
    else if (!strcmp(data_type, "double")) {
        double value;
        sc_out<double>* port = 0;
        writePortSignalValue(value, port, args, "ssd", str);
    }
    else {
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Unexpected token was specified [%s] as 2nd parameter of writePortValue.",
            data_type);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    cout << str.str().c_str() << endl;
    return Py_BuildValue("s", str.str().c_str());

}

/// <summary>Python extended command</summary>
/// <param name="self">pointer of python object</param>
/// <param name="args">pointer of python object</param>
/// <returns>pointer of python object</returns>
static PyObject* readPortValuePy(PyObject* self, PyObject* args)
{
    char *port_name;
    char *data_type;
    std::ostringstream str;

    if (!PyArg_ParseTuple(args, "ss", &port_name, &data_type)) {
        //parse error
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Parse error of readPortValue.\n",
            port_name);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    if (!strcmp(data_type, "bool")) {
        bool value;
        sc_in<bool>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "char")) {
        char value;
        sc_in<char>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "unsigned char")) {
        unsigned char value;
        sc_in<unsigned char>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "short")) {
        short value;
        sc_in<short>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "unsigned short")) {
        unsigned short value;
        sc_in<unsigned short>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!(strcmp(data_type, "int") &&
        strcmp(data_type, "long"))) {
        int value;
        sc_in<int>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!(strcmp(data_type, "unsigned int") &&
        strcmp(data_type, "unsigned long"))) {
        unsigned int value;
        sc_in<unsigned int>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "long long")) {
        long long value;
        sc_in<long long>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "unsigned long long")) {
        unsigned long long value;
        sc_in<unsigned long long>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "float")) {
        float value;
        sc_in<float>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else if (!strcmp(data_type, "double")) {
        double value;
        sc_in<double>* port = 0;
        readPortSignalValue(value, port, args, "ss", str);
    }
    else {
        char error_message_buf[1024];
        sprintf(error_message_buf,
            "Unexpected token was specified [%s] as 2nd parameter of readPortValue.",
            data_type);
        printWarningMessage("main", "main", error_message_buf);
        return Py_BuildValue("s", error_message_buf);
    }

    cout << str.str().c_str() << endl;
    return Py_BuildValue("s", str.str().c_str());

}
#endif//BEFORE_SIGNAL_WRITE_BY_PYTHON

// python extended command definition
static PyMethodDef m_sh_api_def[] = {
    { "sc_start", sc_startPy, METH_VARARGS, "sc_start" },
    { "sc_stop", sc_stopPy, METH_VARARGS, "sc_stop" },
    { "setFreq", setFreqPy, METH_VARARGS, "setFreq" },
    #ifndef BEFORE_SIGNAL_WRITE_BY_PYTHON
    { "writeSignalValue", writeSignalValuePy, METH_VARARGS, "writeSignalValue" },
    { "readSignalValue", readSignalValuePy, METH_VARARGS, "readSignalValue" },
    { "writePortValue", writePortValuePy, METH_VARARGS, "writePortValue" },
    { "readPortValue", readPortValuePy, METH_VARARGS, "readPortValue" },
    #endif//BEFORE_SIGNAL_WRITE_BY_PYTHON
    { NULL, NULL, 0, NULL }
};

/// <summary>set python extended command</summary>
void SetPyExtCmd(void) 
{
    Py_InitModule("SCHEAP", m_sh_api_def);
}
