#! /bin/csh -f
set get_cov_report         #Option to get coverage

if (-e ../../All_simulation_results) then
    rm -rf ../../All_simulation_results
endif
mkdir ../../All_simulation_results

set cov = "branch"

set PRODUCT_LINE_LIST="u2b"
set MODEL_LIST="hage"

######   OSCI LIB   ################
set RE_BUILD_OPTION="y"
foreach product (${PRODUCT_LINE_LIST})
    setenv PRODUCT $product
    foreach model (${MODEL_LIST})
        setenv MODEL $model
        foreach patname (pat_1 pat_2)
            setenv OSCI_USK osci
            source ../setup_osci.csh
            echo
            setenv TIMING_MODE lt
            yes ${RE_BUILD_OPTION} | ./run_all_osci.csh ${patname} ${TIMING_MODE} ${MODEL} ${PRODUCT} ${cov} #memcheck
            set RE_BUILD_OPTION="n"
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE} ..."
            cp -rf ../../log ../../All_simulation_results/log_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../reports ../../All_simulation_results/reports_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../results ../../All_simulation_results/results_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE} ... Finished!"

            setenv TIMING_MODE at
            yes ${RE_BUILD_OPTION} | ./run_all_osci.csh ${patname} ${TIMING_MODE} ${MODEL} ${PRODUCT} ${cov} #memcheck
            set RE_BUILD_OPTION="n"
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE} ..."
            cp -rf ../../log ../../All_simulation_results/log_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../reports ../../All_simulation_results/reports_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../results ../../All_simulation_results/results_${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE}
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${PRODUCT}_${TIMING_MODE} ... Finished!"

        end
        unsetenv MODULE
    end
end
if ($?get_cov_report) then
   ../gen_report/get_coverage.csh HAGE
endif

echo

######   USK LIB   ################
set RE_BUILD_OPTION="y"
foreach product (${PRODUCT_LINE_LIST})
    setenv PRODUCT $product
    foreach model (${MODEL_LIST})
        setenv MODEL $model
        foreach patname (pat_1 pat_2)
            setenv OSCI_USK usk
            source ../setup_usk.csh
            echo
            setenv TIMING_MODE lt
            yes ${RE_BUILD_OPTION} | ./run_all_usk.csh ${patname} ${TIMING_MODE} ${MODEL} ${PRODUCT} ${cov}
            set RE_BUILD_OPTION="n"
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE} ..."
            cp -rf ../../log ../../All_simulation_results/log_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../reports ../../All_simulation_results/reports_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../results ../../All_simulation_results/results_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE} ... Finished!"

            setenv TIMING_MODE at
            yes ${RE_BUILD_OPTION} | ./run_all_usk.csh ${patname} ${TIMING_MODE} ${MODEL} ${PRODUCT} ${cov}
            set RE_BUILD_OPTION="n"
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE} ..."
            cp -rf ../../log ../../All_simulation_results/log_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../reports ../../All_simulation_results/reports_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            cp -rf ../../results ../../All_simulation_results/results_${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE}
            echo "[RUN_ALL_MODE] Get Simulation Result - ${OSCI_USK}_${patname}_${MODEL}_${MODULE}_${PRODUCT}_${TIMING_MODE} ... Finished!"

        end
        unsetenv MODULE
    end
end

echo
echo "****************************************************************************************"
echo "* All Simulation Results are stored at ENV/All_simulation_results directory!           *"
echo "* Model: HAGE                                                                          *"
echo "*======================================================================================*"
echo "* Include 'log', 'reports' and 'results' directories as following formats:             *"
echo "*         + log_<osci_usk>_<patname>_<model>_<module>_<product>_<timing_mode>          *"
echo "*         + reports_<osci_usk>_<patname>_<model>_<module>_<product>_<timing_mode>      *"
echo "*         + results_<osci_usk>_<patname>_<model>_<module>_<product>_<timing_mode>      *"
echo "* Notes:                                                                               *"
echo "*    <osci_usk> is 'osci' or 'usk'                                                     *"
echo "*    <patname> is 'pat_1' or 'pat_2'                                                   *"
echo "*    <model> is 'hage'                                                                 *"
echo "*    <timing_mode> is 'lt' or 'at'                                                     *"
echo "*    <product> is 'u2b'                                                                *"
echo "*======================================================================================*"
echo
