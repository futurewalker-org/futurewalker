# ------------------------------------------
# Futurewalker::Dependency
# ------------------------------------------
set(Library "Dependency")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Event
)

fw_source_group(${LibraryName})
