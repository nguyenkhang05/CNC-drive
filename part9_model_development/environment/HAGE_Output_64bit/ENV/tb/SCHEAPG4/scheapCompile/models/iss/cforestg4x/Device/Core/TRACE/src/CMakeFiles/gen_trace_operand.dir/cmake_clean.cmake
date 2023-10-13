file(REMOVE_RECURSE
  "../include/trace_operand.h"
  "trace_operand_gen.cpp"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/gen_trace_operand.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
