file(REMOVE_RECURSE
  "libforest_scheap.pdb"
  "libforest_scheap.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/forest_scheap.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
