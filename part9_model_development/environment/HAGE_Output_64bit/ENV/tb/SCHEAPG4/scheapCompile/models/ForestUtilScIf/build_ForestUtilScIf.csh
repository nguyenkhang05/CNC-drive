#! /bin/csh -f

source setup.csh
make -f Makefile clean
make -f Makefile compile
cp -rfp lib-ForestUtilScIf/libForestUtilScIf.so   ../../lib/
cp -rfp lib-ForestUtilScIf/libForestUtilScIf.so   ../../lib/libForestUtilScIf.so.0
