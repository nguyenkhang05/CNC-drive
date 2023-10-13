file(REMOVE_RECURSE
  "../include/sregfile.h"
  "sregfile_init.cpp"
  "../test/mock_sregfile.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/gen_sysreg_file_h.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
