// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PointerScopeType.hpp"
#include "Futurewalker.Application.PointerParameter.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp" 
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Pointer scope.
///
class PointerScope : NonCopyable
{
public:
    PointerScope(PassKey<View>, View& view, PointerParameter const& parameter);

    auto GetParameter() const -> PointerParameter const&;

    auto SetResult(Bool const result) -> void;
    auto GetResult(PassKey<View>) const -> Bool;

    static auto DispatchRootView(PassKey<RootView>, View& view, PointerParameter const& parameter) -> Shared<View>;

private:
    View& _view;
    PointerParameter _parameter;
    Bool _result = false;
};
}
}
