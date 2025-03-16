// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ArrangeScopeType.hpp"
#include "Futurewalker.Application.ArrangeParameter.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Arrange scope.
///
class ArrangeScope : NonCopyable
{
public:
    ArrangeScope(PassKey<View>, View& view, ArrangeParameter const& parameter);

    auto GetParameter() -> ArrangeParameter const&;

    auto ArrangeChild(ReferenceArg<View> view, Point<Dp> const& position) -> void;

    auto GetMeasuredSize(ReferenceArg<View> view) -> Size<Dp>;

    static auto ArrangeRootView(PassKey<RootView>, View& view)  -> void;
    
private:
    View& _view;
    ArrangeParameter _parameter;
};
}
}
