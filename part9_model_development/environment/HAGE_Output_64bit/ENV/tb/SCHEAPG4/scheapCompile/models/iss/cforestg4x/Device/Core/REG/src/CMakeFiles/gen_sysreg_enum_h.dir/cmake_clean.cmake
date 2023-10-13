file(REMOVE_RECURSE
  "../include/sreg_enum.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/gen_sysreg_enum_h.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
