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
  Futurewalker::Text
)

if (WIN32)
  fw_link_libraries(${LibraryName} PUBLIC dwmapi shcore)
endif()

if (APPLE)
  fw_link_libraries(${LibraryName} PUBLIC 
  "$<LINK_LIBRARY:FRAMEWORK,AppKit>"
  "$<LINK_LIBRARY:FRAMEWORK,CoreGraphics>"
  "$<LINK_LIBRARY:FRAMEWORK,CoreText>"
  "$<LINK_LIBRARY:FRAMEWORK,Metal>"
  "$<LINK_LIBRARY:FRAMEWORK,MetalKit>"
  "$<LINK_LIBRARY:FRAMEWORK,QuartzCore>")
endif()

fw_source_group(${LibraryName})
