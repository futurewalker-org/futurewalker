# ------------------------------------------
# Futurewalker::Test
# ------------------------------------------
set(Library "Test")
set(LibraryName "Futurewalker-${Library}")

fw_add_interface_library(${LibraryName})

fw_link_libraries(${LibraryName} INTERFACE 
  Futurewalker::Application
)

fw_source_group(${LibraryName})
