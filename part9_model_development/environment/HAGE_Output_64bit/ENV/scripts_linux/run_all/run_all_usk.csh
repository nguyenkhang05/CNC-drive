#! /bin/csh -f

source ../setup_usk.csh
setenv PAT $argv[1]
setenv TIMING_MODE $argv[2]
setenv MODEL $argv[3]
setenv PRODUCT $argv[4]
set cov = $5
setenv OSCI_USK osci

if (($TIMING_MODE !~ "lt" && $TIMING_MODE !~ "at") || ($PAT !~ "pat_1" && $PAT !~ "pat_2")) then
    echo
    echo "Wrong or missing argument(s)!"
    echo "============================="
    echo "Usage:    run_all_usk.csh <pat> <timing_mode> <model> <product_line>"
    echo "              <pat>           : pat_1 | pat_2"
    echo "              <timing_mode>   : lt | at"
    echo "              <model >        : hage"
    echo "              <product_line>  : u2b"
    echo "============================="
    echo
    exit
endif

if ($cov =~ "line") then
    set cov = "line"
else 
    set cov = "branch"
endif
if (-e ../../tb/SCHEAPG4/scheapCompile/build/TOPV02/sim-usk.x) then
    echo
    echo "The SC-HEAP environment is already compiled!"
    echo "============================="
    echo "Need to re-compile? (y/n)"
    echo "============================="
    set cmd = $<
    if (($cmd =~ "y") || ($cmd =~ "Y")) then
        set compile_env
        rm -f ../../tb/SCHEAPG4/scheapCompile/build/TOPV02/sim-usk.x
    endif
else
    set compile_env
endif

set compile_tm
set run_sim

if ($?compile_env) then
    ######Update source code from src folder
    echo
    echo "[RUN_ALL] Copy new source to the environment ..."
    echo
    ######Copy source code of target model
    rm -rf  ../../tb/SCHEAPG4/scheapCompile/models_rvc/HAGE
    cp -fpr ../../../src/HAGE                                                                       ../../tb/SCHEAPG4/scheapCompile/models_rvc/

    ######Copy Python IF
    cp -rfp ../../tb/SCHEAPG4/scheapCompile/models_rvc/DUMMY_MASTER_RVC/PY_DummyMasterRvc.*         ../../tb/SCHEAPG4/scheapCompile/build/PYTHON_IF
    cp -rfp ../../tb/SCHEAPG4/scheapCompile/models_rvc/DUMMY_PERIPHERAL_RVC/PY_DummyPeripheralRvc.* ../../tb/SCHEAPG4/scheapCompile/build/PYTHON_IF
    cp -rfp ../../tb/SCHEAPG4/scheapCompile/models_rvc/HAGE/PY_*.*                                  ../../tb/SCHEAPG4/scheapCompile/build/PYTHON_IF/

    echo
    echo "[RUN_ALL] Copy new source to the environment ... Finished!"
    echo

    if ($cov =~ "branch") then
        foreach dir_name (HAGE)
            echo
            echo "[RUN_ALL] Generate code for branch coverage ..."
            echo
            pushd ../../tb/SCHEAPG4/scheapCompile/models_rvc/${dir_name}
            set listcov=`find . -iname "*.cpp"`
            foreach file ($listcov)
                echo $file
                set file_name = `echo $file | sed "s/\.cpp//g"`
                echo $file_name
                $DUMMYINS_PATH/dummyins.x -o ${file_name}.cpp
                mv ${file_name}_ins.cpp ${file_name}.cpp
                chmod +x ${file_name}.cpp
            end
            popd
        end
    endif

    ###### Compile environment
    echo
    echo "[RUN_ALL] Compiling the SC-HEAP environment ..."
    echo
    pushd ../../tb/SCHEAPG4/scheapCompile/build/TOPV02
    # gmake clean-usk
    gmake release-usk
    popd

    if (-e ../../tb/SCHEAPG4/scheapCompile/build/TOPV02/sim-usk.x) then
        echo
        echo "[RUN_ALL] Compiling the SC-HEAP environment ... Finished!"
        echo
    else 
        echo
        echo "[RUN_ALL] Compiling the SC-HEAP environment ... Failed!"
        echo
        exit
    endif 
endif

if ($?compile_tm) then
    ######Copy Test patterns
    echo
    echo "[RUN_ALL] Copy the test patterns ..."
    echo
    if (! -e ../../pat) then
        mkdir ../../pat
    else
        rm -rf ../../pat/*
    endif
    cp -rf ../../"$PAT"_"$MODEL"/* ../../pat/
    cp -f ../../"$PAT"_"$MODEL"/TM_path_$TIMING_MODE.txt ../../pat/TM_path.txt
    echo
    echo "[RUN_ALL] Copy the test patterns ... Finished!"
    echo

    ######Compile Test patterns
    ######For C TMs only
    if ($PAT =~ "pat_2") then
        echo
        echo "[RUN_ALL] Compiling the C test patterns ..."
        echo
        pushd ../../pat/0_common_files
        setenv COMMON_DIR `pwd`
        popd
        pushd ../gen_mot/
        foreach tm (`cat ../../pat/TM_path.txt | /bin/sed -e "s/#.*//g"` )
            if (-e ../../pat/$tm) then
                set hex_file = `echo ../../pat/$tm | /bin/sed -e "s/\.c/\.hex/g"`
                if (-e $hex_file) then
                    rm -rf $hex_file
                endif
                perl gen_mot.pl -f ../../pat/$tm -i ../../pat/0_common_files -t $TIMING_MODE
                if !(-e $hex_file) then
                    echo
                    echo "[RUN_ALL] Compiling test patterns ... Failed!"
                    echo
                    exit
                endif
            endif 
        end
        popd
        echo
        echo "[RUN_ALL] Compiling the test patterns ... Finished!"
        echo
    endif
endif

if ($?run_sim) then
   ######Generate simulation environment
    echo
    echo "[RUN_ALL] Generate simulation environment ..."
    echo
    if (! -e ../../sim) then
        mkdir ../../sim
    else
        rm -rf ../../sim/*
    endif
    if (! -e ../../log) then
        mkdir ../../log
    else
        rm -rf ../../log/*
    endif
    if (! -e ../../results) then
        mkdir ../../results
    else
        rm -rf ../../results/*
    endif
    if (! -e ../../reports) then
        mkdir ../../reports
    else
        rm -rf ../../reports/*
    endif
    pushd ../gen_sim

    # Select config file
    # cp -f ./${PRODUCT}/configfile_${TIMING_MODE}.skl ./configfile_${TIMING_MODE}.skl

    perl gen_sim_usk.pl -m ${MODEL}_${PRODUCT} -t $TIMING_MODE -p $PAT -exe ../tb/SCHEAPG4/scheapCompile/build/TOPV02/sim-usk.x #-memcheck ${MODEL}
    popd
    echo
    echo "[RUN_ALL] Generate simulation environment ... Finished!"
    echo

    ######Run simulation and check result
    echo
    echo "[RUN_ALL] Run simulation and check result ..."
    echo
    pushd ../../sim
    ./run_all.csh
    popd
    echo
    echo "[RUN_ALL] Run simulation and check result ... Finished!"
    echo

    ######Generate report
    echo
    echo "[RUN_ALL] Generate report ..."
    echo
    pushd ../gen_report
    perl gen_report.pl -m $MODEL -t $TIMING_MODE -p $PAT -o $OSCI_USK
    popd
    echo
    echo "[RUN_ALL] Generate report ... Finished!"
    echo

    cat ../../reports/summary_report.txt

    ######Update sim folder for Windows simulation
    echo
    echo "[RUN_ALL] Prepare Windows simulation environment ..."
    echo
    pushd ../../sim/
    foreach sim_x (`find -iname "sim-usk.x"`)
        rm -f $sim_x
    end
    foreach trace_log (`find -iname "trace.log"`)
       rm -f $trace_log
    end
    popd
    if (-e ../../sim_usk_${PAT}_${MODEL}_${PRODUCT}_${TIMING_MODE}) then
        rm -rf ../../sim_usk_${PAT}_${MODEL}_${PRODUCT}_${TIMING_MODE}
    endif
    mkdir ../../sim_usk_${PAT}_${MODEL}_${PRODUCT}_${TIMING_MODE}
    cp -rf ../../sim/* ../../sim_usk_${PAT}_${MODEL}_${PRODUCT}_${TIMING_MODE}/
    echo
    echo "[RUN_ALL] Prepare Windows simulation environment ... Finished!"
    echo
endif
