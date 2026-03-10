# Futurewalker

A cross-platform GUI framework written in C++23.  

> [!NOTE]  
> Futurewalker is currently in early development phase.  
> If you have interest in the project, play around with it and provide feedback!  

<img src=docs/images/20260309_Gallery_Color.png width=412> <img src=docs/images/20260309_Gallery_Buttons.png width=412>

## Overview

* Class based retained-mode API
* Composable, responsive layout system
* Customizable styling system
* Coroutine support
* GPU accelerated graphics by Skia
* Styleless core + styled components

## Project goals

The primary goal of this project is to crate a modern GUI framework for complex tools applications.

More specific goals are:

* Supporting wide range of features required by complex native applications
* Incorporating modern GUI concepts like responsive layouts, gestures, etc
* Focusing on flexibility, customizability and extensibility
* Supporting multiple platforms, ideally all of popular ones
* Decent performance and energy efficiency
* Better safety story compared to other C++ frameworks
* Better async story compared to other C++ frameworks

Non-goals:

* Native look and feel
* In-game UIs
* Externally hosted UIs like audio software plugins

## Project status

Currently the main focus of development is building underlying machinery for higher level components.  
A few of UI controls and layouts are available at this moment.

### Supported platforms 

Currently only supports Windows 11.

| OS | Support Status |
| ---- | ---- |
| Windows 10 | ❔ Undecided |
| Windows 11 | 🚧 In Development |
| macOS | 🚧 Planned for 2026~ |
| iOS | 📝 Planned for 2027~ |
| Android | 📝 Planned for 2027~ |
| Linux | ❔ Undecided |
| Web | ❔ Undecided |

## Building

> [!NOTE]  
> This is for building tests and examples in this repository.  
> There's no proper way to develop applications yet.  

You can build tests and examples with following steps:

1. Install Conan package manager

```
pip3 install conan
```

2. Setup local package for Skia

```
git clone https://github.com/mocabe/conan-skia.git
conan remote add conan_skia ./conan-skia
```

3. Build the project

```
git clone https://github.com/mocabe/futurewalker.git
cd futurewalker
conan build . -pr:h=./conan/vs2022_x86_64_Debug.profile --build=missing
```

## Documentation

> [!NOTE]  
> TODO

## License

Licensed under Mozilla Public License 2.0

