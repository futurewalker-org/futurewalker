# ------------------------------------------
# Futurewalker::Graphics
# ------------------------------------------
set(Library "Graphics")
set(LibraryName "Futurewalker-${Library}")

fw_add_library(${LibraryName})

fw_link_libraries(${LibraryName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Core
  Futurewalker::Base
  Futurewalker::Color
  Futurewalker::Geometry
  Futurewalker::Unit
  Futurewalker::Text
  Futurewalker::Library::skia
)

if (WIN32)
  fw_link_libraries(${LibraryName} PUBLIC d3d11 d3d12 dcomp)
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
