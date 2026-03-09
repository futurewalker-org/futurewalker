# ------------------------------------------
# HelloWorld
# ------------------------------------------
set(Example "HelloWorld")
set(ExampleName "Example-${Example}")

fw_add_binary(${ExampleName} Windowed)

fw_link_libraries(${ExampleName} PUBLIC 
  Futurewalker::Config
  Futurewalker::Application
  Futurewalker::Component::Lamp
  Futurewalker::Main
)