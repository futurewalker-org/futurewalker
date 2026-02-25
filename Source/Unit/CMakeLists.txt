# ------------------------------------------
# Futurewalker::Unit
# ------------------------------------------
set(Library "Unit")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Geometry
)

fw_source_group(${LibraryName})
