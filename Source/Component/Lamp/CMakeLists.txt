# ------------------------------------------
# Futurewalker::Component::Lamp
# ------------------------------------------
set(Library "Lamp")
set(LibraryName "Futurewalker-Component-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Application
)

fw_source_group(${LibraryName})
