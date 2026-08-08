// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaSvgDocument.hpp"
#include "Futurewalker.Graphics.SkiaScene.hpp"

#include "Futurewalker.Core.DynamicCastFunction.hpp"

#include <include/core/SkStream.h>
#include <modules/svg/include/SkSVGRenderContext.h>

namespace FW_GRAPHICS_DETAIL_NS
{
auto SkiaSvgDocument::LoadFromStream(Shared<InputStream> const& stream) -> Unique<SkiaSvgDocument>
{
    struct SkiaStream : public SkStream
    {
        Shared<InputStream> _stream;

        explicit SkiaStream(Shared<InputStream> const& stream)
          : _stream(stream)
        {
        }

        auto read(void* buffer, size_t size) -> size_t override
        {
            if (!_stream)
            {
                return 0;
            }

            auto const span = std::span<std::byte>(reinterpret_cast<std::byte*>(buffer), size);
            if (auto const readBytes = _stream->Read(span))
            {
                return static_cast<size_t>(*readBytes);
            }
            return 0;
        }

        auto isAtEnd() const -> bool override
        {
            if (!_stream)
            {
                return true;
            }
            if (auto const position = _stream->GetPosition())
            {
                if (auto const endPosition = _stream->SetPosition(0, SeekPosition::End))
                {
                    _stream->SetPosition(*position, SeekPosition::Begin);
                    return *position == *endPosition;
                }
            }
            return false;
        }
    };

    auto skStream = SkiaStream(stream);
    if (auto svg = SkSVGDOM::MakeFromStream(skStream))
    {
        return Unique<SkiaSvgDocument>::Make(PassKey<SkiaSvgDocument>(), svg);
    }
    return {};
}

SkiaSvgDocument::SkiaSvgDocument(PassKey<SkiaSvgDocument>, sk_sp<SkSVGDOM> const& svg)
  : _svg(svg)
{
}

auto SkiaSvgDocument::Draw(Scene& scene, Size<Dp> const& viewport) -> void
{
    if (auto skiaScene = DynamicCastFunction::TryAs<SkiaScene>(&scene))
    {
        if (auto skiaCanvas = skiaScene->GetCanvas())
        {
            auto containerSize = SkSize::Make(static_cast<SkScalar>(viewport.width), static_cast<SkScalar>(viewport.height));
            _svg->setContainerSize(containerSize);
            _svg->render(static_cast<SkCanvas*>(skiaCanvas));
        }
    }
}

auto SkiaSvgDocument::GetIntrinsicSize(Size<Dp> const& viewport) const -> Size<Dp>
{
    auto lengthContext = SkSVGLengthContext(SkSize::Make(static_cast<SkScalar>(viewport.width), static_cast<SkScalar>(viewport.height)));
    auto intrinsicSize = _svg->getRoot()->intrinsicSize(lengthContext);
    return Size<Dp>(intrinsicSize.width(), intrinsicSize.height());
}
}
