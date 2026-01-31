// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformSkiaCompositionSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp" 
#include "Futurewalker.Graphics.SkiaScene.hpp" 

#include "Futurewalker.Base.Debug.hpp"

#include <dcomp.h>

#include <include/core/SkColorSpace.h>
#include <include/core/SkCanvas.h>
#include <include/gpu/ganesh/GrDirectContext.h>
#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/d3d/GrD3DTypes.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
auto AddPresentationBuffer(PlatformSkiaGraphicsDeviceWin& graphicsDevice, PlatformDCompositionDeviceWin& dcompDevice, IPresentationManager& manager, IntPx width, IntPx height)
{
    auto texture = graphicsDevice.CreateTexture(width, height);
    auto sharedTextureHandle = graphicsDevice.CreateSharedHandle(texture);
    auto sharedTexture = dcompDevice.CreateSharedResourceFromHandle(sharedTextureHandle.Get());

    Microsoft::WRL::ComPtr<IPresentationBuffer> presentationBuffer;
    manager.AddBufferFromResource(sharedTexture.Get(), &presentationBuffer);
    return std::make_pair(texture, presentationBuffer);
}
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::Make(
  Shared<PlatformSkiaGraphicsDeviceWin> const& device,
  Shared<PlatformDCompositionDeviceWin> const& dcompDevice,
  IntPx const width,
  IntPx const height) -> Shared<PlatformSkiaCompositionSwapChainSurfaceWin>
{
    auto surface = Shared<PlatformSkiaCompositionSwapChainSurfaceWin>::Make(PassKey<PlatformSkiaCompositionSwapChainSurfaceWin>(), device, dcompDevice, width, height);
    surface->_self = surface;
    return surface;
}

PlatformSkiaCompositionSwapChainSurfaceWin::PlatformSkiaCompositionSwapChainSurfaceWin(
  PassKey<PlatformSkiaCompositionSwapChainSurfaceWin>,
  Shared<PlatformSkiaGraphicsDeviceWin> const& device,
  Shared<PlatformDCompositionDeviceWin> const& dcompDevice,
  IntPx const width,
  IntPx const height)
  : _device(device)
  , _dcompDevice(dcompDevice)
{
    if (_dcompDevice)
    {
        BuildResources();
        Resize(width, height);
        _dcompDevice->AddDeviceObject(GetSelf());
    }
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::Resize(IntPx const width, IntPx const height) -> Bool
{
    if (width <= 0 || height <= 0)
    {
        FW_DEBUG_ASSERT(false);
        return false;
    }

    if (_width != width || _height != height)
    {
        _width = width;
        _height = height;
        UpdateSourceRect();
    }
    return true;
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::Draw(Function<void(Scene& scene)> func) -> Bool
{
    auto const presentId = _presentationManager->GetNextPresentId();
    auto const index = presentId % static_cast<UINT>(_presentationBuffers.size());

    if (_presentationBuffers[index].buffer)
    {
        auto const waitResult = ::WaitForSingleObject(_presentationBuffers[index].bufferAvailableEvent.get(), INFINITE);
        if (waitResult != WAIT_OBJECT_0)
        {
            FW_DEBUG_ASSERT(false);
            _presentationBuffers[index] = {};
            return false;
        }
    }

    auto const bufferDesc = _presentationBuffers[index].resource ? _presentationBuffers[index].resource->GetDesc() : D3D12_RESOURCE_DESC();
    if (bufferDesc.Width != static_cast<UINT64>(_width) || bufferDesc.Height != static_cast<UINT>(_height))
    {
        _presentationBuffers[index] = {};
        _presentationBuffers[index] = AllocPresentationBuffer();
    }

    if (auto const surface = _presentationBuffers[index].surface)
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
        _device->FlushAndSubmitSurface(surface.get(), SkSurfaces::BackendSurfaceAccess::kPresent, GrSyncCpu::kYes);
    }

    auto hr = _presentationSurface->SetBuffer(_presentationBuffers[index].buffer.Get());
    if (FAILED(hr))
    {
        FW_DEBUG_ASSERT(false);
    }

    hr = _presentationManager->Present();
    if (hr == PRESENTATION_ERROR_LOST)
    {
        NotifyDeviceLost();
        return false;
    }
    else if (FAILED(hr))
    {
        FW_DEBUG_ASSERT(false);
        return false;
    }
    return true;
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::NotifyDeviceLost() -> void
{
    if (_dcompDevice)
    {
        _dcompDevice->NotifyDeviceLost();
    }
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::HandleDeviceLost() -> void
{
    ClearResources();
    BuildResources();
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::GetSwapChain() -> Microsoft::WRL::ComPtr<IUnknown>
{
    return _dcompSurface;
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::SetDelegate(Delegate delegate) -> void
{
    _delegate = std::move(delegate);
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::GetPresentationManager() -> Microsoft::WRL::ComPtr<IPresentationManager>
{
    return _presentationManager;
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::GetPresentationSurface() -> Microsoft::WRL::ComPtr<IPresentationSurface>
{
    return _presentationSurface;
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::GetSelf() -> Shared<PlatformSkiaCompositionSwapChainSurfaceWin>
{
    return _self.Lock();
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::AllocPresentationBuffer() -> PresentationBuffer
{
    auto [resource, buffer] = AddPresentationBuffer(*_device, *_dcompDevice, *_presentationManager.Get(), _width, _height);

    auto bufferAvailableEvent = winrt::handle();
    buffer->GetAvailableEvent(bufferAvailableEvent.put());

    auto const resourceDesc = resource->GetDesc();

    // NOTE: GrD3DTextureResourceInfo takes ID3D12Resource* as constructor argument but does NOT add refcount internally.
    auto const resourceInfo = GrD3DTextureResourceInfo(
      GrSafeComAddRef(resource.Get()),
      nullptr,
      D3D12_RESOURCE_STATE_RENDER_TARGET,
      resourceDesc.Format,
      resourceDesc.SampleDesc.Count,
      resourceDesc.MipLevels,
      resourceDesc.SampleDesc.Quality);

    auto const backendTexture = GrBackendTexture(static_cast<int>(resourceDesc.Width), static_cast<int>(resourceDesc.Height), resourceInfo);
    auto surface = _device->CreateBackendTextureSurface(backendTexture);
    return {
        .resource = std::move(resource),
        .buffer = std::move(buffer),
        .bufferAvailableEvent = std::move(bufferAvailableEvent),
        .surface = std::move(surface),
    };
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::UpdateSourceRect() -> void
{
    if (_presentationSurface)
    {
        auto const sourceRect = RECT {
            .left = 0,
            .top = 0,
            .right = static_cast<LONG>(_width),
            .bottom = static_cast<LONG>(_height),
        };
        _presentationSurface->SetSourceRect(&sourceRect);
    }
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::ClearResources() -> void
{
    _presentationBuffers.clear();
    _dcompSurface.Reset();
    _fence.Reset();
    _presentationSurface.Reset();
    _presentationManager.Reset();
}

auto PlatformSkiaCompositionSwapChainSurfaceWin::BuildResources() -> void
{
    if (_dcompDevice)
    {
        auto const surfaceHandle = _dcompDevice->CreateSurfaceHandle();
        _presentationManager = _dcompDevice->CreatePresentationManager();
        _presentationManager->CreatePresentationSurface(surfaceHandle.Get(), &_presentationSurface);
        _presentationManager->GetPresentRetiringFence(IID_PPV_ARGS(&_fence));
        _dcompSurface = _dcompDevice->CreateSurfaceFromHandle(surfaceHandle.Get());
        _presentationSurface->SetAlphaMode(DXGI_ALPHA_MODE_PREMULTIPLIED);
        _presentationBuffers.resize(3);
        UpdateSourceRect();
    }
}
}
