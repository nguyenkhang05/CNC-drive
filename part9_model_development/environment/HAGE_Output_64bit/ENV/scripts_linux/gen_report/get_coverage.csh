#!/bin/csh -f

    ##Setting this before using
    set target_model=$1 #=> setting in Run_all_execution.csh
    if ($target_model =~ "") then
        echo "[Error] Please enter model name"
        exit
    else
#        set target_model=$target_model:agu
    endif

    pushd ../../
    mkdir -p All_simulation_results/report_coverage
    # Get coverage
    if (! -d tb/SCHEAPG4/scheapCompile/models_rvc/$target_model) then
        echo "[Error] The model name does not exist"
        exit
    endif
    #Find file list
    pushd ../src/$target_model/
    set listcov=`find ./ -iname "*.cpp"`
    if ($#listcov == 0) then
        echo "[Error] The model name does not exist"
        exit
    endif
    popd

    pushd tb/SCHEAPG4/scheapCompile/models_rvc/$target_model/
    echo "====================================================" > ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
    echo "=                  SUMMARY INFO                    =" >> ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
    echo "====================================================" >> ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
    echo "Filename Coverage" | awk '{printf " %-30s %s\n",$1,$2}' >> ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
    echo "----------------------------------------------------" >> ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
    foreach file ($listcov)
        set file_name = `echo $file | sed "s/\.cpp//g" | sed "s/^.*\///g"`
        if ($file_name == "PY_${target_model}") then
            pushd ../../build/PYTHON_IF/
            set cov_percent = `gcov ${file_name}.cpp -o ../../lib-modelsO3/PYTHON_IF | grep -A 1 "${file_name}.cpp" | grep "%" | sed "s/.*://g" | sed "s/%.*/%/g"`
            popd
        else
            set cov_percent = `gcov ${file_name}.cpp -o ../../lib-modelsO3/$target_model | grep -A 1 "${file_name}.cpp" | grep "%" | sed "s/.*://g" | sed "s/%.*/%/g"`
        endif
        echo "${file_name}.cpp $cov_percent" | awk '{printf " %-30s %s\n",$1,$2}' >> ../../../../../All_simulation_results/report_coverage/${target_model}_summary_info.txt
        if (-e ../../../../../All_simulation_results/report_coverage/${file_name}.cpp.gcov_uncover) then
            rm -rf ../../../../../All_simulation_results/report_coverage/${file_name}.cpp.gcov_uncover
        endif
        if ($file_name == "PY_${target_model}") then
            pushd ../../build/PYTHON_IF/
            grep -R -A 1 -B 1 "#####" ${file_name}.cpp.gcov > ../../../../../All_simulation_results/report_coverage/${file_name}.cpp.gcov_uncover
            popd
        else
            grep -R -A 1 -B 1 "#####" ${file_name}.cpp.gcov > ../../../../../All_simulation_results/report_coverage/${file_name}.cpp.gcov_uncover
        endif
    end
    popd

    pushd ../src/$target_model/
    # Get line_num summary
    echo "Filename Revision Codeline" | awk '{printf "\n %-30s %-10s %s\n",$1,$2,$3}' >> ../../ENV/All_simulation_results/report_coverage/${target_model}_summary_info.txt
    echo "----------------------------------------------------" >> ../../ENV/All_simulation_results/report_coverage/${target_model}_summary_info.txt
    set list=`find ./ -type f -iname "*.*"`
    foreach file ($list)
        set line_num = `cat $file | sed -e "s/[ +]//g" | grep -v ^$ | wc -l`
        # set line_num = `cat $file | sed -e "s/[ +]//g" | grep -v "^//" | grep -v ^$ | wc -l`
        set file_name = `echo $file | sed "s/^.*\///g"`
        set rev = `grep '^// *$Id.*\$' $file | grep "$file_name" | sed -e 's/\/\/.*[^$]*Id: //g' | awk '{printf "%s",$2}'`
        if ($rev == "") then
            set rev = "-"
        endif
        echo "${file_name} $rev $line_num" | awk '{printf " %-30s %-10s %s\n",$1,$2,$3}' >> ../../ENV/All_simulation_results/report_coverage/${target_model}_summary_info.txt
    end
    popd
    popd
    pushd ../../All_simulation_results/report_coverage/
    rm -f ${target_model}_uncoverage_report.txt
    echo "=============== Uncoverage report of ${target_model} model ===============" > ${target_model}_uncoverage_report.txt
    foreach fname (*.gcov_uncover)
        set disname = `echo $fname | sed -e "s/\.gcov_uncover//g"`
        echo "File name: $disname" >> ${target_model}_uncoverage_report.txt
        echo "=====================================" >> ${target_model}_uncoverage_report.txt
        cat $fname >> ${target_model}_uncoverage_report.txt
        echo "" >> ${target_model}_uncoverage_report.txt
        echo "" >> ${target_model}_uncoverage_report.txt
        rm -f $fname
    end
    popd
    echo Done!

