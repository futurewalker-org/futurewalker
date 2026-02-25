# ------------------------------------------
# Futurewalker::Attribute
# ------------------------------------------
set(Library "Attribute")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Base
  Futurewalker::Event
)

fw_source_group(${LibraryName})
