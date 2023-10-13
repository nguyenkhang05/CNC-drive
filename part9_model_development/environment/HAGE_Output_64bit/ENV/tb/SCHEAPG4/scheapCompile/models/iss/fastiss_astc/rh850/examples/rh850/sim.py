#! python

#------------------------------------------------------------------------------
# Copyright (C) Australian Semiconductor Technology Company. 2019.
# All Rights Reserved.
#
# This is unpublished proprietary source code of the Australian Semiconductor
# Technology Company (ASTC).  The copyright notice does not evidence any actual
# or intended publication of such source code.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------

import vlab
import sysc
import time
import rh850.debug
import rh850.execution_tracer
import rh850.translation_tracer
import rh850.disassembler
import rh850.analysis_delegate

#------------------------------------------------------------------------------
# Create and install analysis delegate
#------------------------------------------------------------------------------
 
delegate = rh850.analysis_delegate.AnalysisDelegate()

#------------------------------------------------------------------------------
# Initialise global variables
#------------------------------------------------------------------------------

args = None
initial_wall_time = None

#------------------------------------------------------------------------------
# VLAB platform simulation phase action handlers
#------------------------------------------------------------------------------

def start_of_simulation(bp):
  # Global variables
  global args
  global initial_wall_time

  # Grab handle to core instance
  cpu = vlab.get_instance("CPU1")
  
  # Attach tracers (if required)
  if (args["trace-disable"] == False):
    cpu.execution_tracer.attach(cpu.obj)
  if (args["enable-translation-trace"]):
    cpu.translation_tracer.attach(cpu.obj)

  # Record start time (if required)
  if (args["show-performance"]):
    initial_wall_time = time.time()

def end_of_simulation(bp):
  # Global variables
  global args
  global initial_wall_time

  # Grab handle to core instance
  cpu = vlab.get_instance("CPU1")

  # Detach tracers (if required)
  if (args["trace-disable"] == False):
    cpu.execution_tracer.detach()
  if (args["enable-translation-trace"]):
    cpu.translation_tracer.detach()
    
  # Calculate performance (if required)
  if (args["show-performance"]):
    instruction_count = cpu.obj.get_instruction_count()
    print "Instructions: %d" % instruction_count
    print "MIPS: %f" % ((instruction_count / (time.time() - initial_wall_time)) / 1000000)

  # Produce memory dump (if required)
  if (args["show-ram-dump"]):
    vlab.show_memory(args["ram-start-address"], args["ram-size"], endian="little")

#------------------------------------------------------------------------------
# Build example platform
#------------------------------------------------------------------------------

# Set the simulator name
vlab.properties(name="rh850_example_platform",
                extensions={"sc_writer_policy": "unchecked"})

# Parse simulator configuration    
vlab.option(
  "ram-size",
  payload="int",
  default=0,
  format_spec="hex",
  doc="Specifies RAM size in bytes.")
vlab.option(
  "ram-start-address",
  payload="int",
  default=0x00000000,
  format_spec="hex",
  doc="Specifies RAM start address.")
vlab.option(
  "rom-size",
  payload="int",
  default=0,
  format_spec="hex",
  doc="Specifies ROM size in bytes.")
vlab.option(
  "rom-start-address",
  payload="int",
  default=0x00000000,
  format_spec="hex",
  doc="Specifies ROM start address.")
vlab.option(
  "block-size",
  payload="int",
  default=100,
  doc="Translation block size limit.")
vlab.option(
  "quantum-period",
  payload="int",
  default=1,
  doc="Quantum period duration specified in mS.")
vlab.option(
  "blocking-tlm",
  payload="bool",
  default=False,
  doc="Controls whether non-blocking (False) or blocking (True) TLM transactions are issued.")
vlab.option(
  "tlm-response-error-as-error",
  payload="bool",
  default=False,
  doc="Controls whether the core reports TLM response errors as either errors (True) or warnings (False).")
vlab.option(
  "clock-frequency",
  payload="int",
  default=100000000,
  doc="Clock frequency specified in Hertz.")
vlab.option(
  "trace-disable",
  payload="bool",
  default=False,
  doc="Causes the production of an execution trace information to be disabled.")
vlab.option(
  "trace-file-name",
  payload="string",
  default="",
  doc="Specifies the file to which execution trace output should be directed. An empty string signifies that execution trace should be directed to standard output.") 
vlab.option(
  "show-performance",
  payload="bool",
  default=False,
  doc="Causes the runtime performance to be measured and the MIPS value to be displayed at the end of simulation.")
vlab.option(
  "show-ram-dump",
  payload="bool",
  default=False,
  doc="Causes RAM contents to be displayed at the end of simulation.")
vlab.option(
  "run",
  short_name="r",
  payload="float",
  default=-1,
  doc="If specified, then run for the specified duration (expressed in seconds) and terminate. If not specified or if a negative duration is specified then control will be returned to the parent script or console after the platform build step completes.")
vlab.option(
  "translation-engine",
  payload="int",
  default=3,
  doc="Identifies the binary translation engine to use. [0-3]")
vlab.option(
  "adaptive-count",
  payload="int",
  default=5,
  doc="Block execution threshold at which blocks are submitted for binary translation.")
vlab.option(
  "translation-partition-size",
  payload="int",
  default=4096,
  doc="Size of partitions used for translation cache flush management.")
vlab.option(
  "enable-variable-trace",
  payload="bool",
  default=False,
  doc="Enable global variable trace to VCD.")
vlab.option(
  "enable-function-trace",
  payload="bool",
  default=False,
  doc="Enable function trace.")
vlab.option(
  "enable-translation-trace",
  payload="bool",
  default=False,
  doc="Enable translation trace.")
vlab.enable_standard_options(trace=True, image=True)
args = vlab.parse_args()

# Check options
if (args["trace-disable"] and (args["trace-file-name"] != "")):
  vlab.exit("Error: --trace-disable and --trace-file-name are mutually exclusive")
  
# Instantiate the core
cpu_component = vlab.component(name="rh850_core", description="rh850.description")
cpu = vlab.instantiate(cpu_component, "CPU1", args=[vlab.NAME, args["show-performance"] == True])

# Initialise the VLAB debug infrastructure
(cpu.facade, cpu.target) = rh850.debug.initialise(cpu, 0)

# Install disassembler(s)
cpu.rh850e3_disassembler = rh850.disassembler.rh850e3_disassembler()
cpu.rh850e3_disassembler.initialise()
vlab.add_disassembler(cpu.rh850e3_disassembler)

# Create a TLM router
num_initiators = 0
if (args["rom-size"] > 0):
  num_initiators += 1
if (args["ram-size"] > 0):
  num_initiators += 1 
router_component = vlab.component("tlm_router_32", module="vlab.components")
router = vlab.instantiate(router_component, "ROUTER1", args=["router", 1, num_initiators])
initiator_id = 0
if (args["rom-size"] > 0):
  router.obj.add_address_mapping(initiator_id, args["rom-start-address"], args["rom-size"], args["rom-start-address"])
  initiator_id += 1
if (args["ram-size"] > 0):
  router.obj.add_address_mapping(initiator_id, args["ram-start-address"], args["ram-size"], args["ram-start-address"])
  initiator_id += 1

# Create a TLM external memory
memory_component = vlab.component("tlm_memory_32", module="vlab.components")
if (args["rom-size"] > 0):
  rom = vlab.instantiate(memory_component, "ROM1", args=["ROM1", args["rom-size"], 0, True])
if (args["ram-size"] > 0):
  ram = vlab.instantiate(memory_component, "RAM1", args=["RAM1", args["ram-size"]])

# Configure the core
vlab.assign((cpu, "block_size"), args["block-size"])
vlab.assign((cpu, "quantum_period"), sysc.sc_time(args["quantum-period"], sysc.SC_MS))
vlab.assign((cpu, "blocking_tlm"), args["blocking-tlm"])  
vlab.assign((cpu, "tlm_response_error_as_error"), args["tlm-response-error-as-error"])  
vlab.assign((cpu, "translation_engine"), args["translation-engine"])
vlab.assign((cpu, "adaptive_count"), args["adaptive-count"])
vlab.assign((cpu, "translation_partition_size"), args["translation-partition-size"])

# Connect the core, memory and router
vlab.connect((cpu, "bus_initiator"), (router, "target_socket", 0))
initiator_id = 0
if (args["rom-size"] > 0):
  vlab.connect((router, "initiator_socket", initiator_id), (rom, "target_socket"))
  initiator_id += 1
if (args["ram-size"] > 0):
  vlab.connect((router, "initiator_socket", initiator_id), (ram, "target_socket"))
  initiator_id += 1

# Stub external connections

vlab.connect(vlab.STUB, (cpu, "clock"), payload="uint64", default=args["clock-frequency"])

# Add phase action handlers
vlab.add_breakpoint(vlab.trigger.phase("start_of_simulation"), start_of_simulation, hidden=True)
vlab.add_breakpoint(vlab.trigger.phase("end_of_simulation"), end_of_simulation, hidden=True)

# Create tracers (if required)
if (args["trace-disable"] == False):
  cpu.execution_tracer = rh850.execution_tracer.rh850_execution_tracer(args["trace-file-name"])
if (args["enable-translation-trace"]):
  cpu.translation_tracer = rh850.translation_tracer.rh850_translation_tracer()

# Handle standard options
vlab.elaborate()
vlab.handle_standard_options(args, trace=True, image=True)

# Enable variable trace (if required)
if (args["enable-variable-trace"] and (len(vlab.get_symbols(kind='object')) > 0)):
  vlab.enable_analysis(vlab.analysis.view.variable_trace)

# Enable function trace (if required)
if (args["enable-function-trace"] and (len(vlab.get_symbols(kind='func')) > 0)):
  vlab.enable_analysis(vlab.analysis.view.function_trace)

# Run simulation (if required)
if (args["run"] >= 0):
  vlab.run(args["run"], "s", blocking=True)
  if (args["enable-function-trace"]):
    vlab.display_analysis(vlab.analysis.view.function_trace(), True)
  vlab.exit()

#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
