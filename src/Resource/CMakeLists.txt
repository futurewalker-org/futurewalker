# ------------------------------------------
# Futurewalker::Resource
# ------------------------------------------
set(Library "Resource")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Base
)

fw_source_group(${LibraryName})
