// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformSkiaSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.SkiaScene.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Exception.hpp"

#include <include/core/SkColorSpace.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/d3d/GrD3DTypes.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
void ThrowOnFail(auto const hr)
{
    if (FAILED(hr))
    {
        throw Exception(ErrorCode::Failure);
    }
}
void ThrowOnFalse(auto const& p)
{
    if (!p)
    {
        throw Exception(ErrorCode::Failure);
    }
}
}

///
/// @brief Make new instance.
///
/// @param device 
/// @param width 
/// @param height 
///
auto PlatformSkiaSwapChainSurfaceWin::Make(Shared<PlatformSkiaGraphicsDeviceWin> const& device, IntPx const width, IntPx const height) -> Shared<PlatformSkiaSwapChainSurfaceWin>
{
    auto surface = Shared<PlatformSkiaSwapChainSurfaceWin>::Make(PassKey<PlatformSkiaSwapChainSurfaceWin>(), device, width, height);
    surface->_self = surface;
    return surface;
}

///
/// @brief Constructor.
///
/// @param device Graphics device object. Should not be null.
/// @param width Width of swap chain. Should be >= 1.
/// @param height Height of swap chain. Should be >= 1.
///
PlatformSkiaSwapChainSurfaceWin::PlatformSkiaSwapChainSurfaceWin(PassKey<PlatformSkiaSwapChainSurfaceWin>, Shared<PlatformSkiaGraphicsDeviceWin> const& device, IntPx const width, IntPx const height)
  : _device {device}
  , _width {width}
  , _height {height}
{
    FW_DEBUG_ASSERT(device);

    if (_width <= 0 || _height <= 0)
    {
        FW_DEBUG_ASSERT(false);
        _width = IntPx::Max(1, _width);
        _height = IntPx::Max(1, _height);
    }

    if (_device)
    {
        BuildResources();
        _device->AddDeviceObject(GetSelf());
    }
}

///
/// @brief Resize buffers.
///
/// @param width New width of swap chain. Should be >= 1.
/// @param height New height of swap chain. Should be >= 1.
///
/// @return Returns true when successfully resized surface.
///
auto PlatformSkiaSwapChainSurfaceWin::Resize(IntPx const width, IntPx const height) -> Bool
{
    if (_width <= 0 || _height <= 0)
    {
        FW_DEBUG_ASSERT(false);
        return false;
    }

    if (_width != width || _height != height)
    {
        _width = width;
        _height = height;
        return BuildResources();
    }
    return true;
}

///
/// @brief Draw content.
///
/// @param func
///
auto PlatformSkiaSwapChainSurfaceWin::Draw(Function<void(Scene& scene)> func) -> Bool
{
    try
    {
        if (!HasSwapChain())
        {
            return false;
        }

        auto const index = _swapChain->GetCurrentBackBufferIndex();
        ThrowOnFail(_fence->SetEventOnCompletion(_swapChainSurfaces[index].fenceValue, NULL));

        if (auto const surface = _swapChainSurfaces[index].surface)
        {
            if (auto const canvas = surface->getCanvas())
            {
                auto const saveCount = canvas->save();
                canvas->clear(SkColor4f());

                auto const scene = Shared<SkiaScene>::Make();
                scene->SetCanvas(canvas);

                try
                {
                    if (func)
                    {
                        func(*scene);
                    }
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }

                canvas->restoreToCount(saveCount);
            }
            _device->FlushAndSubmitSurface(surface.get(), SkSurfaces::BackendSurfaceAccess::kPresent, GrSyncCpu::kNo);
        }

        auto const presentParams = DXGI_PRESENT_PARAMETERS();
        auto const hr = _swapChain->Present1(0, 0, &presentParams);
        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            NotifyDeviceLost();
            return false;
        }
        else
        {
            ThrowOnFail(hr);
        }

        _currentFenceValue += 1;
        ThrowOnFalse(_device->SignalFence(_fence.Get(), _currentFenceValue));
        _swapChainSurfaces[index].fenceValue = _currentFenceValue;
        return true;
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
    return false;
}

///
/// @brief Notify device loss.
///
auto PlatformSkiaSwapChainSurfaceWin::NotifyDeviceLost() -> void
{
    if (_delegate.deviceLost)
    {
        try
        {
            _delegate.deviceLost(*this);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }

    if (_device)
    {
        _device->NotifyDeviceLost();
    }
}

///
/// @brief Handle device loss.
///
auto PlatformSkiaSwapChainSurfaceWin::HandleDeviceLost() -> void
{
    ClearResources();

    if (!BuildResources())
    {
        return;
    }

    if (_delegate.deviceRestored)
    {
        try
        {
            _delegate.deviceRestored(*this);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }
}

///
/// @brief Get swap chain.
///
auto PlatformSkiaSwapChainSurfaceWin::GetSwapChain() -> Microsoft::WRL::ComPtr<IUnknown>
{
    return _swapChain;
}

///
/// @brief Set delegate.
///
/// @param delegate
///
auto PlatformSkiaSwapChainSurfaceWin::SetDelegate(Delegate delegate) -> void
{
    _delegate = std::move(delegate);
}

///
/// @brief Get self reference.
///
auto PlatformSkiaSwapChainSurfaceWin::GetSelf() -> Shared<PlatformSkiaSwapChainSurfaceWin>
{
    return _self.Lock();
}

///
/// @brief Returns true if swap chain is available.
///
auto PlatformSkiaSwapChainSurfaceWin::HasSwapChain() const -> Bool
{
    return (_device && _swapChain && _fence);
}

///
/// @brief Build swap chain.
///
auto PlatformSkiaSwapChainSurfaceWin::BuildSwapChain() -> Bool
{
    if (!HasSwapChain())
    {
        try
        {
            _swapChain = _device->CreateSwapChain(_width, _height);
            ThrowOnFalse(_swapChain);

            _fence = _device->CreateFence();
            ThrowOnFalse(_fence);

            ThrowOnFalse(RebuildSurfaces());

            return true;
        }
        catch (...)
        {
            ClearResources();
        }
        return false;
    }
    return true;
}

///
/// @brief Resize buffers.
///
auto PlatformSkiaSwapChainSurfaceWin::ResizeBuffers() -> Bool
{
    if (HasSwapChain())
    {
        try
        {
            WaitIdle();

            auto desc = DXGI_SWAP_CHAIN_DESC {};
            ThrowOnFail(_swapChain->GetDesc(&desc));

            if (_width != desc.BufferDesc.Width || _height != desc.BufferDesc.Height)
            {
                FW_DEBUG_ASSERT(_width > 0 && _height > 0);

                // Surfaces will become invalid after resize.
                _swapChainSurfaces.clear();

                auto const hr = _swapChain->ResizeBuffers(0, static_cast<UINT>(_width), static_cast<UINT>(_height), DXGI_FORMAT_UNKNOWN, 0);
                if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
                {
                    return false;
                }
                else
                {
                    ThrowOnFail(hr);
                }

                ThrowOnFalse(RebuildSurfaces());
            }
            return true;
        }
        catch (...)
        {
            ClearResources();
        }
        return false;
    }
    return false;
}

///
/// @brief Rebuild surfaces.
///
auto PlatformSkiaSwapChainSurfaceWin::RebuildSurfaces() -> Bool
{
    if (HasSwapChain())
    {
        try
        {
            WaitIdle();

            _swapChainSurfaces.clear();

            auto desc = DXGI_SWAP_CHAIN_DESC {};
            ThrowOnFail(_swapChain->GetDesc(&desc));

            // Create surface for each buffer.
            for (auto i = SInt64(0); i < desc.BufferCount; ++i)
            {
                Microsoft::WRL::ComPtr<ID3D12Resource> resource;
                ThrowOnFail(_swapChain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(resource.GetAddressOf())));
                ThrowOnFalse(resource);

                auto const resourceDesc = resource->GetDesc();

                // NOTE: GrD3DTextureResourceInfo takes ID3D12Resource* as constructor argument but does NOT add refcount internally.
                auto const resourceInfo = GrD3DTextureResourceInfo(
                  GrSafeComAddRef(resource.Get()),
                  nullptr,
                  D3D12_RESOURCE_STATE_PRESENT,
                  desc.BufferDesc.Format,
                  resourceDesc.SampleDesc.Count,
                  resourceDesc.MipLevels,
                  resourceDesc.SampleDesc.Quality);

                auto const backendTexture = GrBackendTexture(static_cast<int>(desc.BufferDesc.Width), static_cast<int>(desc.BufferDesc.Height), resourceInfo);

                auto surface = _device->CreateBackendTextureSurface(backendTexture);
                ThrowOnFalse(surface);

                _swapChainSurfaces.push_back(
                  SwapChainSurface {
                      .resource = resource,
                      .surface = surface,
                  });
            }
            return true;
        }
        catch (...)
        {
            _swapChainSurfaces.clear();
        }
        return false;
    }
    return true;
}

///
/// @brief Wait until all operations are completed.
///
auto PlatformSkiaSwapChainSurfaceWin::WaitIdle() -> void
{
    if (HasSwapChain())
    {
        try
        {
            _device->FlushAndSubmit(GrSyncCpu::kYes);

            for (auto const& swapChain : _swapChainSurfaces)
            {
                if (FAILED(_fence->SetEventOnCompletion(swapChain.fenceValue, NULL)))
                {
                    FW_DEBUG_ASSERT(false);
                }
            }
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }
}

///
/// @brief 
///
auto PlatformSkiaSwapChainSurfaceWin::ClearResources() -> void
{
    _swapChainSurfaces.clear();
    _fence.Reset();
    _swapChain.Reset();
}

///
/// @brief
///
auto PlatformSkiaSwapChainSurfaceWin::BuildResources() -> Bool
{
    if (HasSwapChain())
    {
        return ResizeBuffers();
    }
    else
    {
        return BuildSwapChain();
    }
}
}
