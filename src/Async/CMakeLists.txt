# ------------------------------------------
# Futurewalker::Async
# ------------------------------------------
set(Library "Async")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Base
)

fw_source_group(${LibraryName})
