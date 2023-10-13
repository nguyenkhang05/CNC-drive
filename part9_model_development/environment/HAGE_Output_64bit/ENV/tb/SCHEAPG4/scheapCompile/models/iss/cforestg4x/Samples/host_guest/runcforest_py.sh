#!/bin/sh

CFG3M_PATH=/home/nishihr/v850/cforest_g3m/sim
$CFG3M_PATH/cforest_g3m --scr test.py --debug rw cache --cycle --max 5000 --rom-wait 4 --core-type G3M10 --no-self-area --penum 2
