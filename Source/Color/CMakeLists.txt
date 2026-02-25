# ------------------------------------------
# Futurewalker::Color
# ------------------------------------------
set(Library "Color")
set(LibraryName "Futurewalker-${Library}")

fw_add_interface_library(${LibraryName})

fw_link_libraries(${LibraryName} INTERFACE 
  Futurewalker::Config
  Futurewalker::Core
)

fw_source_group(${LibraryName})
