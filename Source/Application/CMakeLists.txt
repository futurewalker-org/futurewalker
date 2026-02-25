# ------------------------------------------
# Futurewalker::Application
# ------------------------------------------
set(Library "Application")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Base
  Futurewalker::Action
  Futurewalker::Event
  Futurewalker::Attribute
  Futurewalker::Animation
  Futurewalker::Graphics
)

if (WIN32)
  fw_link_libraries(${LibraryName} PUBLIC dwmapi shcore)
endif()

fw_source_group(${LibraryName})
