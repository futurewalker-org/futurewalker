# ------------------------------------------
# Futurewalker::Animation
# ------------------------------------------
set(Library "Animation")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Dependency
)

fw_source_group(${LibraryName})
