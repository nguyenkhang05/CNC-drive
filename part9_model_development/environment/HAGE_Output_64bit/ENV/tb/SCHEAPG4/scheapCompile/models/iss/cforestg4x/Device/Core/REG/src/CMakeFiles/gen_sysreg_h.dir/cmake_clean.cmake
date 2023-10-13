file(REMOVE_RECURSE
  "../include/sreg.h"
  "../test/mock_sreg.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/gen_sysreg_h.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
