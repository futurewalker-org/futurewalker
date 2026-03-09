# ------------------------------------------
# Futurewalker::Text
# ------------------------------------------
set(Library "Text")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Unit
)

fw_source_group(${LibraryName})
