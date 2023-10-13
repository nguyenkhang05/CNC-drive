cmake_minimum_required( VERSION 3.2 )

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_e2xfcc2.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh11_e2xfcc2.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy init_CForest_g4mh21_1pe.py ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${TOP_SOURCE_DIR}/Samples/python_script/CForest/init_CForest_g4mh_1pe.py
  DEPENDS ${TOP_SOURCE_DIR}/Samples/python_script/CForest/init_CForest_g4mh11_1pe.py
  DEPENDS ${TOP_SOURCE_DIR}/Samples/python_script/CForest/init_CForest_g4mh11_e2xfcc2.py
  DEPENDS ${TOP_SOURCE_DIR}/Samples/python_script/CForest/init_CForest_g4mh20_1pe.py
  DEPENDS ${TOP_SOURCE_DIR}/Samples/python_script/CForest/init_CForest_g4mh21_1pe.py
  WORKING_DIRECTORY ${TOP_SOURCE_DIR}/Samples/python_script/CForest
  COMMENT "Copy init_CForest.py for G4MHx."
)
add_custom_target(
  copy_init_python_g4mh ALL
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_e2xfcc2.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py
)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.bat ${CMAKE_CURRENT_BINARY_DIR}/test.hex
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_py.bat ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_py.sh ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh11_py.bat ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh11_py.sh ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh11_e2xfcc2_py.bat ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh11_e2xfcc2_py.sh ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh20_py.bat ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh20_py.sh ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh21_py.bat ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy runcforest_g4mh21_py.sh ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy test.hex ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS runcforest_py.bat runcforest_py.sh runcforest_g4mh11_py.bat runcforest_g4mh11_py.sh runcforest_g4mh11_e2xfcc2_py.bat runcforest_g4mh11_e2xfcc2_py.sh runcforest_g4mh20_py.bat runcforest_g4mh20_py.sh runcforest_g4mh21_py.bat runcforest_g4mh21_py.sh test.hex
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Copy sample for G4MHx."
)
add_custom_target(
  copy_sample_g4mh ALL
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.sh ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.bat ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.sh ${CMAKE_CURRENT_BINARY_DIR}/test.hex
)

if( ${TARGET}  MATCHES "^CFORESTG4X$" )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh_1pe.py DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_1pe.py DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh11_e2xfcc2.py DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh20_1pe.py DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/init_CForest_g4mh21_1pe.py DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.bat DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_py.sh DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.bat DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_py.sh DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.bat DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh11_e2xfcc2_py.sh DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.bat DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh20_py.sh DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.bat DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/runcforest_g4mh21_py.sh DESTINATION sample COMPONENT samples )
  install( FILES ${CMAKE_CURRENT_BINARY_DIR}/test.hex DESTINATION sample COMPONENT samples )
endif( )
