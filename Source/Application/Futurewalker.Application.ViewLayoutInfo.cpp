// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewLayoutInfo.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief 
///
auto ViewLayoutInfo::GetFrameRect() const -> Rect<Dp>
{
    return _frameRect;
}

///
/// @brief
///
auto ViewLayoutInfo::GetContentRect() const -> Rect<Dp>
{
    return _contentRect;
}

///
/// @brief
///
auto ViewLayoutInfo::InvalidatePath() -> Bool
{
    if (!_measureRequired && !_measuring && !_arranging)
    {
        _measureRequired = true;
        return true;
    }
    return false;
}

///
/// @brief
///
auto ViewLayoutInfo::BeginMeasure(MeasureParameter const& parameter) -> Bool
{
    if (!_measuring)
    {
        _measureParameter = parameter;

        if (IsMeasureRequired() || IsMeasureNeeded(parameter))
        {
            _measuring = true;
            _measuredSizeSet = false;
            _arrangeRequired = true;
            return true;
        }
    }
    return false;
}

///
/// @brief
///
auto ViewLayoutInfo::EndMeasure() -> void
{
    if (_measuring)
    {
        _measuring = false;
        _measureRequired = false;
    }
}

///
/// @brief
///
auto ViewLayoutInfo::IsMeasuring() const -> Bool
{
    return _measuring;
}

///
/// @brief
///
auto ViewLayoutInfo::BeginArrange(ArrangeParameter const& parameter) -> Bool
{
    if (!_arranging)
    {
        _frameRect = parameter.GetNewFrameRect();
        _contentRect = Rect<Dp>({}, _frameRect.GetSize());

        if (IsArrangeRequired())
        {
            _arranging = true;
            return true;
        }
    }
    return false;
}

///
/// @brief
///
auto ViewLayoutInfo::EndArrange() -> void
{
    if (_arranging)
    {
        _arranging = false;
        _arrangeRequired = false;
    }
}

///
/// @brief
///
auto ViewLayoutInfo::IsArranging() const -> Bool
{
    return _arranging;
}

///
/// @brief
///
auto ViewLayoutInfo::GetMeasuredSize() const -> Size<Dp>
{
    if (_measuredSizeSet)
    {
        FW_DEBUG_ASSERT(_measuredSize.widthConstraints == _measureParameter.GetWidthConstraints());
        FW_DEBUG_ASSERT(_measuredSize.heightConstraints == _measureParameter.GetHeightConstraints());
        return _measuredSize.size;
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

///
/// @brief
///
/// @param parameter
/// @param size
///
auto ViewLayoutInfo::SetMeasuredSize(MeasureParameter const& parameter, Size<Dp> const& size) -> void
{
    auto forceConstraints = [](const auto& c, const auto& v) {
        const auto constrained = AxisConstraints::Constrain(c, v);
        FW_DEBUG_ASSERT(v == constrained); // Size should satisfy incoming constraints.
        return constrained;
    };

    const auto width = forceConstraints(parameter.GetWidthConstraints(), size.GetWidth());
    const auto height = forceConstraints(parameter.GetHeightConstraints(), size.GetHeight());

    _measuredSize = {
      .widthConstraints = parameter.GetWidthConstraints(),
      .heightConstraints = parameter.GetHeightConstraints(),
      .size = Size<Dp>(width, height),
    };
    _measuredSizeSet = true;
}

///
/// @brief
///
auto ViewLayoutInfo::GetLayoutDirection() const -> LayoutDirection
{
    return _layoutDirection;
}

///
/// @brief
///
/// @param layoutDirection
///
auto ViewLayoutInfo::SetLayoutDirection(LayoutDirection const layoutDirection) -> void
{
    _layoutDirection = layoutDirection;
}

///
/// @brief
///
auto ViewLayoutInfo::GetRawLayoutDirection() const -> ViewLayoutDirection
{
    return _rawLayoutDirection;
}

///
/// @brief
///
/// @param layoutDirection
///
auto ViewLayoutInfo::SetRawLayoutDirection(ViewLayoutDirection const layoutDirection) -> void
{
    _rawLayoutDirection = layoutDirection;
}

///
/// @brief
///
auto ViewLayoutInfo::IsMeasureRequired() const -> Bool
{
    return _measureRequired;
}

///
/// @brief Check if constraints differ from previous ones.
///
/// @param parameter
///
auto ViewLayoutInfo::IsMeasureNeeded(MeasureParameter const& parameter) const -> Bool
{
    const auto widthConstraints = parameter.GetWidthConstraints();
    const auto heightConstraints = parameter.GetHeightConstraints();
    return _measuredSize.widthConstraints != widthConstraints || _measuredSize.heightConstraints != heightConstraints;
}

///
/// @brief
///
auto ViewLayoutInfo::IsArrangeRequired() const -> Bool
{
    return _arrangeRequired;
}
}
