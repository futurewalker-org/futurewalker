// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Window;

///
/// @brief Behavior of windows.
///
enum class WindowBehavior
{
    ///
    /// @brief A behavior of normal windows.
    ///
    Normal = 0,

    ///
    /// @brief A behavior of dialog windows.
    ///
    Dialog,

    ///
    /// @brief A behavior of popup windows.
    ///
    Popup,

    ///
    /// @brief A behavior of tooltip windows.
    ///
    Tooltip,
};

///
/// @brief Various areas on window.
///
enum class WindowArea
{
    None = 0,

    ///
    /// @brief Total area of title bar.
    ///
    TitleBar,

    ///
    /// @brief Customizable area in title bar.
    ///
    TitleBarContent,

    ///
    /// @brief Content area of the window.
    ///
    Content,
};

///
/// @brief Various areas on window.
///
enum class WindowRegion
{
    ///
    /// @brief Outside of the window.
    ///
    None = 0,

    ///
    /// @brief Content area of the window.
    ///
    Content,

    ///
    /// @brief Title bar area of the window.
    ///
    TitleBar,
};

///
/// @brief Describes how window's background should be drawn.
///
enum class WindowBackgroundStyle
{
    ///
    /// @brief Use system default.
    ///
    System,

    ///
    /// @brief Use user defined solid color.
    ///
    /// @note Need to specify background color in WindowOptions.
    ///
    Solid,

    ///
    /// @brief Window should have transparent background.
    ///
    Transparent,
};

///
/// @brief Options for creating Window object.
///
struct WindowOptions
{
    ///
    /// @brief Behavior of the window.
    ///
    /// Determines how will the new window behave.
    ///
    WindowBehavior behavior = WindowBehavior::Normal;

    ///
    /// @brief Background style of window.
    ///
    /// Determines how window's background will be drawn.
    ///
    WindowBackgroundStyle backgroundStyle = WindowBackgroundStyle::System;

    ///
    /// @brief Background color of window.
    ///
    RGBAColor backgroundColor = RGBAColor(0, 0, 0, 1);

    ///
    /// @brief Owner of this window.
    ///
    /// The new window will become owned window, which have several properties:
    /// 1. Owned window will always be placed above its owner.
    /// 2. When owner widnow is destroyed, owned window will also be destroyed.
    /// 3. When owned window is modal, its direct and indirect owners will be disabled.
    ///
    /// @note Each window can have at most one owned window.
    /// @note Specifying owner window which already have owned window is not allowed.
    /// @note Owner window should not be the one disabled by another modal window.
    ///
    Shared<Window> owner = nullptr;

    ///
    /// @brief Is the window closable.
    ///
    /// When this value is set to `true`, the window will be closable.
    ///
    Bool closable = true;

    ///
    /// @brief Is the window resizable.
    ///
    /// When this value is set to `true`, the window will be resizable.
    ///
    /// @see `Window::SetMaxSize()`
    /// @see `Window::SetMinSize()`
    ///
    Bool resizable = true;

    ///
    /// @brief Is the window minimizable.
    ///
    /// When this value is set to `true`, the window will be minimizable.
    ///
    Bool minimizable = true;

    ///
    /// @brief Does the window have frame.
    ///
    /// Set this value to `false` to make the window frame-less.
    ///
    Bool hasFrame = true;

    ///
    /// @brief Does the window have title bar.
    ///
    /// Set this value to `false` to make the window titlebar-less.
    ///
    Bool hasTitleBar = true;
};
}
}
