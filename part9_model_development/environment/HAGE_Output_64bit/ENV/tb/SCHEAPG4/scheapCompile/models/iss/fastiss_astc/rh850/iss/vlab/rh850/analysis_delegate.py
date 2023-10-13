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
import analysis_interface

#------------------------------------------------------------------------------
# Analysis delegate
#------------------------------------------------------------------------------

class AnalysisDelegate(object):
  def __init__(self):
    self.cores = []
    try:
      vlab.add_breakpoint(vlab.trigger.phase('end_of_elaboration'), action=self.initialise, hidden=True)
    except RuntimeError:
      self.initialise()
    vlab.analysis.scenario.callgrind(delegate=self, update=True)
    vlab.analysis.scenario.function_profile(delegate=self, update=True)
    vlab.analysis.scenario.function_coverage(delegate=self, update=True)
    vlab.analysis.scenario.variable_trace(delegate=self, update=True)
    vlab.analysis.scenario.statement_coverage(delegate=self, update=True)
    vlab.analysis.scenario.code_coverage(delegate=self, update=True)
      
  def initialise(self, bp=None):
    for core_object in vlab.get_cores():
      try:      
        # Create analysis interface
        core_object.analysis_interface = analysis_interface.create_analysis_interface(core_object.obj)
        
        # Indicate that core supports fast analysis
        self.cores.append((core_object.fullname, True))
      except sysc.sc_report, e:
        # Report any SystemC errors
        vlab.exit(e.what())
      except Exception, e:
        # Indicate that core does not supports fast analysis
        vlab.log.warning("%s does not support fast analysis, analysis disabled" % core_object.fullname)
        self.cores.append((core_object.fullname, False))

    # Work around to enable fast profiling without this python operation
    vlab.standard_analysis_scenarios._get_function_return_address = lambda x: x

  def supported_cores(self):
    return self.cores

  def get_analysis_interface(self, core):
    return vlab.get_instance(core).analysis_interface

  def branch_mnemonics(self, core):
    return []

#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
