add_executable(resource-compiler
    main.cpp
)
target_link_libraries(resource-compiler
    Futurewalker::Library::XercesC
    Futurewalker::Library::boost
    Futurewalker::Library::icu)
