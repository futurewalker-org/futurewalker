// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.HitTestScopeType.hpp"
#include "Futurewalker.Application.HitTestParameter.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp" 
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Hit test scope.
///
class HitTestScope : NonCopyable
{
public:
    HitTestScope(PassKey<View>, View& view, HitTestParameter const& parameter);

    auto HitTestChild(ReferenceArg<View> view) -> Shared<View>;

    auto GetParameter() const -> HitTestParameter const&;

    auto SetHit(Bool const result) -> void;
    auto GetHit(PassKey<View>) const -> Bool;

    static auto HitTestView(PassKey<View>, View& view, HitTestParameter const& parameter) -> Shared<View>;

private:
    View& _view;
    HitTestParameter _parameter;
    Bool _hit = false;
    Bool _handled = false;
};
}
}
