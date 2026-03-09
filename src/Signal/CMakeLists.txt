# ------------------------------------------
# Futurewalker::Signal
# ------------------------------------------
set(Library "Signal")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Async
)

fw_source_group(${LibraryName})
