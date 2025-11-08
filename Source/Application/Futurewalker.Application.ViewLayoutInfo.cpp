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
    if (!_measuring && !_arranging)
    {
        _lastMeasureParameter = parameter;

        auto const measureRequired = IsMeasureRequired();
        auto const measureNeeded = IsMeasureNeeded(parameter);
        if (measureRequired || measureNeeded)
        {
            _measuring = true;
            _measuredSizeSet = false;
            _arrangeRequired = true;

            if (measureRequired)
            {
                _measuredIntrinsicSizeSet = false;
            }
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
    if (!_arranging && !_measuring)
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
    auto const lastParameterWidth = _lastMeasureParameter.GetWidthConstraints();
    auto const lastParameterHeight = _lastMeasureParameter.GetHeightConstraints();

    if (_measuredIntrinsicSizeSet)
    {
        if (_measuredIntrinsicSize.widthConstraints == lastParameterWidth && _measuredIntrinsicSize.heightConstraints == lastParameterHeight)
        {
            return _measuredIntrinsicSize.size;
        }
    }

    if (_measuredSizeSet)
    {
        FW_DEBUG_ASSERT(_measuredSize.widthConstraints == lastParameterWidth);
        FW_DEBUG_ASSERT(_measuredSize.heightConstraints == lastParameterHeight);
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

    auto const parameterWidth = parameter.GetWidthConstraints();
    auto const parameterHeight = parameter.GetHeightConstraints();

    auto const width = forceConstraints(parameterWidth, size.GetWidth());
    auto const height = forceConstraints(parameterHeight, size.GetHeight());

    _measuredSize = {
        .widthConstraints = parameterWidth,
        .heightConstraints = parameterHeight,
        .size = Size<Dp>(width, height),
    };
    _measuredSizeSet = true;

    auto const unboundedLooseWidth = !parameterWidth.IsBounded() && parameterWidth.GetMin() == 0;
    auto const unboundedLooseHeight = !parameterHeight.IsBounded() && parameterHeight.GetMin() == 0;
    if (unboundedLooseWidth && unboundedLooseHeight)
    {
        _measuredIntrinsicSize = _measuredSize;
        _measuredIntrinsicSizeSet = true;
    }
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

    if (_measuredIntrinsicSizeSet)
    {
        if (_measuredIntrinsicSize.widthConstraints == widthConstraints && _measuredIntrinsicSize.heightConstraints == heightConstraints)
        {
            return false;
        }
    }

    if (_measuredSizeSet)
    {
        return _measuredSize.widthConstraints != widthConstraints || _measuredSize.heightConstraints != heightConstraints;
    }
    return true;
}

///
/// @brief
///
auto ViewLayoutInfo::IsArrangeRequired() const -> Bool
{
    return _arrangeRequired;
}
}
