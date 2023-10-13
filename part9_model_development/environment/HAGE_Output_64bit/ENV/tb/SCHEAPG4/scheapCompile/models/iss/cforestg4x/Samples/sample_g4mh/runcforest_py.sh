#!/bin/sh

CF_BIN_PATH=../bin
CF_CONFIG_PATH=.
$CF_BIN_PATH/cforestg4x --msglvl ERR --scr $CF_CONFIG_PATH/init_CForest_g4mh_1pe.py --enable-ic --debug rw cache --hex test.hex
