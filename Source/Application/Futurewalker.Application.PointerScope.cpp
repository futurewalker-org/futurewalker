// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PointerScope.hpp"
#include "Futurewalker.Application.View.hpp"

#include <ranges> 

namespace FW_DETAIL_NS
{
///
/// @brief Construct pointer scope.
///
/// @param view
/// @param parameter
///
PointerScope::PointerScope(PassKey<View>, View& view,  PointerParameter const& parameter)
  : _view {view}
  , _parameter {parameter}
{
}

///
/// @brief
///
auto PointerScope::GetParameter() const -> PointerParameter const&
{
    return _parameter;
}

///
/// @brief
///
auto PointerScope::SetResult(Bool const result) -> void
{
    _result = result;
}

///
/// @brief
///
auto PointerScope::GetResult(PassKey<View>) const -> Bool
{
    return _result;
}

///
/// @brief
///
auto PointerScope::DispatchRootView(PassKey<RootView>, View& view, PointerParameter const& parameter) -> Shared<View>
{
    return view.EnterPointerScope({}, parameter);
}
}
