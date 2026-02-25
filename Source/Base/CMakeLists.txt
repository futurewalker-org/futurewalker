# ------------------------------------------
# Futurewalker::Base
# ------------------------------------------
set(Library "Base")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
)

if (WIN32)
  fw_link_libraries(${LibraryName} PUBLIC WindowsApp)
endif()

fw_source_group(${LibraryName})
