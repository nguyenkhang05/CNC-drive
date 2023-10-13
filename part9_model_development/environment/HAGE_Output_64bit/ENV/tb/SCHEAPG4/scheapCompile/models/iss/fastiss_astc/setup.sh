#------------------------------------------------------------------------------
# Copyright (C) Australian Semiconductor Technology Company. 2012.
# All Rights Reserved.
#
# This is unpublished proprietary source code of the Australian Semiconductor
# Technology Company (ASTC).  The copyright notice does not evidence any actual
# or intended publication of such source code.
#------------------------------------------------------------------------------

# Set project root directory (parent of component directory)
export TOPDIR=$(cd $(dirname ${BASH_SOURCE}); pwd)

#------------------------------------------------------------------------------
# Check PMS is installed and configured correctly
#------------------------------------------------------------------------------

isa_compiler --version >& /dev/null 
rc=$?
if [[ $rc != 0 ]]; then
  echo "Error: Unable to run ISA Compiler"
  echo "       Ensure PMS has been setup correctly and valid licenses are available"
  return $rc
fi
 
#------------------------------------------------------------------------------
# Check VLAB is installed and configured correctly
#------------------------------------------------------------------------------

# Check that VLAB is working
vlab --version >& /dev/null 
rc=$?
if [[ $rc != 0 ]]; then
  echo "Error: Unable to run VLAB"
  echo "       Ensure PMS has been setup correctly and valid licenses are available"
  return $rc
fi

# Check that the VLAB_HOME environment variable has been set
if [ -z "${VLAB_HOME}" ]; then
  echo "Error: Ensure PMS has been setup correctly and valid licenses are available"
  return 1
fi

#------------------------------------------------------------------------------
# Set up paths
#------------------------------------------------------------------------------

# Pick-up the runtime and model libraries for VWorks OSCAR
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${TOPDIR}/iss/lib/usk-${FASTISS_PLATFORM}/v850e2m
export VLAB_PATH=${VLAB_PATH}:${TOPDIR}/iss/lib/usk-${FASTISS_PLATFORM}

# Pick up the example platform
export VLAB_PATH=${VLAB_PATH}:${TOPDIR}/iss/examples/v850e2m

#------------------------------------------------------------------------------
# End of file
#------------------------------------------------------------------------------
