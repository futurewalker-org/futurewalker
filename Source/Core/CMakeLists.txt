# ------------------------------------------
# Futurewalker::Core
# ------------------------------------------
set(Library "Core")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Library::boost
  Futurewalker::Library::fmt
  Futurewalker::Library::icu
)

fw_source_group(${LibraryName})
