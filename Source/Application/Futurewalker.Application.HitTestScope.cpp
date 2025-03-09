// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.HitTestScope.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Construct hit test scope.
///
/// @param view
/// @param parameter
///
HitTestScope::HitTestScope(PassKey<View>, View& view, HitTestParameter const& parameter)
  : _view {view}
  , _parameter {parameter}
{
}

///
/// @brief
///
auto HitTestScope::HitTestChild(ReferenceArg<View> view) -> Shared<View>
{
    if (view)
    {
        auto parameter = HitTestParameter();
        parameter.SetPosition(view->AncestorToLocalPoint(_parameter.GetPosition(), _view));
        return view->EnterHitTestScope({}, parameter);
    }
    return nullptr;
}

///
/// @brief
///
auto HitTestScope::GetParameter() const -> HitTestParameter const&
{
    return _parameter;
}

///
/// @brief
///
auto HitTestScope::SetHit(Bool const hit) -> void
{
    _hit = hit;
}

///
/// @brief
///
auto HitTestScope::GetHit(PassKey<View>) const -> Bool
{
    return _hit;
}

///
/// @brief
///
auto HitTestScope::HitTestView(PassKey<View>, View& view, HitTestParameter const& parameter) -> Shared<View>
{
    return view.EnterHitTestScope({}, parameter);
}
}
