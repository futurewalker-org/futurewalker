# ------------------------------------------
# Futurewalker::Event
# ------------------------------------------
set(Library "Event")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Async
  Futurewalker::Signal
)

fw_source_group(${LibraryName})
