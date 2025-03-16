// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.RootViewLayoutInfo.hpp"

namespace FW_DETAIL_NS
{
auto RootViewLayoutInfo::SetSize(Size<Dp> const& size) -> Bool
{
    if (_size != size)
    {
        _size = size;
        return true;
    }
    return false;
}

auto RootViewLayoutInfo::GetSize() const -> Size<Dp>
{
    return _size;
}
}
