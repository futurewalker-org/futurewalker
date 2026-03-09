# ------------------------------------------
# Futurewalker::Main
# ------------------------------------------
set(Library "Main")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Application
)

fw_source_group(${LibraryName})
