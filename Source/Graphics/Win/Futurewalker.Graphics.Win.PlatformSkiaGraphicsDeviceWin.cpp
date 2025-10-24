// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaCompositionSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Exception.hpp"

#include <include/gpu/ganesh/GrBackendSurface.h>
#include <include/gpu/ganesh/d3d/GrD3DBackendContext.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>
#include <include/core/SkColorSpace.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
auto constexpr SurfaceColorType = SkColorType::kBGRA_8888_SkColorType;
auto constexpr SwapChainFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
auto constexpr SwapChainSampleDesc = DXGI_SAMPLE_DESC {.Count = 1, .Quality = 0};
}

///
/// @brief Make graphics device.
///
/// @param device D3D12Device to use
///
auto PlatformSkiaGraphicsDeviceWin::Make(Shared<PlatformD3D12DeviceWin> const& d3d12Device) -> Shared<PlatformSkiaGraphicsDeviceWin>
{
    auto graphicsDevice = Shared<PlatformSkiaGraphicsDeviceWin>::Make(PassKey<PlatformSkiaGraphicsDeviceWin>(), d3d12Device);
    graphicsDevice->_self = graphicsDevice;
    return graphicsDevice;
}

///
/// @brief Constructor.
///
/// @param d3d12Device
///
PlatformSkiaGraphicsDeviceWin::PlatformSkiaGraphicsDeviceWin(PassKey<PlatformSkiaGraphicsDeviceWin>, Shared<PlatformD3D12DeviceWin> const& d3d12Device)
  : _d3d12Device {d3d12Device}
{
    if (!_d3d12Device)
    {
        throw Exception(ErrorCode::InvalidArgument);
    }

    _adapter = _d3d12Device->GetAdapter();
    _device = _d3d12Device->GetDevice();

    if (!BuildCommandQueue())
    {
        throw Exception(ErrorCode::Failure);
    }

    if (!BuildGrContext())
    {
        throw Exception(ErrorCode::Failure);
    }

    _d3d12Device->AddDeviceObject(GetSelf());
}

///
/// @brief Create swap chain based surface.
///
/// @param[in] dcompDevice DComposition device.
/// @param[in] width Width of surface.
/// @param[in] height Height of surface.
///
auto PlatformSkiaGraphicsDeviceWin::MakeSwapChainSurface(Shared<PlatformDCompositionDeviceWin> const& dcompDevice, IntPx const width, IntPx const height) -> Shared<PlatformSwapChainSurfaceWin>
{
    auto surface = Shared<PlatformSwapChainSurfaceWin>();
    if (dcompDevice->IsPresentationSupported())
    {
        surface = PlatformSkiaCompositionSwapChainSurfaceWin::Make(GetSelf(), dcompDevice, width, height);
    }

    if (!surface)
    {
        FW_DEBUG_LOG_WARNING("PlatformSkiaGraphicsDeviceWin: DComposition swap chain surface is not supported. Falling back to regular swap chain surface.");
        surface = PlatformSkiaSwapChainSurfaceWin::Make(GetSelf(), width, height);
    }
    return surface;
}

///
/// @brief Notify operation was failed due to device loss.
///
auto PlatformSkiaGraphicsDeviceWin::NotifyDeviceLost() -> void
{
    if (_d3d12Device)
    {
        _d3d12Device->NotifyDeviceLost();
    }
}

///
/// @brief Handles device loss.
///
auto PlatformSkiaGraphicsDeviceWin::HandleDeviceLost() -> void
{
    ClearResources();
    BuildResources();

    for (auto& weakDeviceObject : _deviceObjects)
    {
        if (auto deviceObject = weakDeviceObject.Lock())
        {
            deviceObject->HandleDeviceLost();
        }
    }
}

///
/// @brief 
///
/// @param deviceObject 
///
auto PlatformSkiaGraphicsDeviceWin::AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void
{
    _deviceObjects.push_back(deviceObject);
}

///
/// @brief 
///
/// @param width 
/// @param height 
///
auto PlatformSkiaGraphicsDeviceWin::CreateTexture(IntPx const width, IntPx const height) -> Microsoft::WRL::ComPtr<ID3D12Resource>
{
    if (_d3d12Device)
    {
        if (auto const device = _d3d12Device->GetDevice())
        {
            D3D12_RESOURCE_DESC desc {};
            desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            desc.Alignment = 0;
            desc.Width = static_cast<UINT>(width);
            desc.Height = static_cast<UINT>(height);
            desc.DepthOrArraySize = 1;
            desc.MipLevels = 1;
            desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

            D3D12_CLEAR_VALUE clearValue {};
            clearValue.Format = desc.Format;
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 0.0f;

            Microsoft::WRL::ComPtr<ID3D12Resource> texture;
            auto defaultHeap = D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
            auto const hr = device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_SHARED, &desc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue, IID_PPV_ARGS(&texture));
            if (SUCCEEDED(hr) && texture)
            {
                return texture;
            }
        }
    }
    return {};
}

///
/// @brief Create swap chain.
///
/// @param[in] width Width of swap chain.
/// @param[in] height Height of swap chain
///
auto PlatformSkiaGraphicsDeviceWin::CreateSwapChain(IntPx const width, IntPx const height) -> Microsoft::WRL::ComPtr<IDXGISwapChain3>
{
    auto factory = Microsoft::WRL::ComPtr<IDXGIFactory2>();
    auto hr = ::CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
    if (FAILED(hr) || !factory)
    {
        return {};
    }

    if (width <= 0 || height <= 0)
    {
        return {};
    }

    auto const desc = DXGI_SWAP_CHAIN_DESC1 {
        .Width = static_cast<UINT>(width),
        .Height = static_cast<UINT>(height),
        .Format = SwapChainFormat,
        .Stereo = FALSE,
        .SampleDesc = SwapChainSampleDesc,
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 3,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED,
        .Flags = 0,
    };

    auto swapChain = Microsoft::WRL::ComPtr<IDXGISwapChain1>();
    hr = factory->CreateSwapChainForComposition(_commandQueue.Get(), &desc, nullptr, swapChain.GetAddressOf());
    if (SUCCEEDED(hr) && swapChain)
    {
        auto swapChain3 = Microsoft::WRL::ComPtr<IDXGISwapChain3>();
        hr = swapChain->QueryInterface(swapChain3.GetAddressOf());
        if (SUCCEEDED(hr) && swapChain3)
        {
            return swapChain3;
        }
    }
    return {};
}

///
/// @brief Create fence.
///
auto PlatformSkiaGraphicsDeviceWin::CreateFence() -> Microsoft::WRL::ComPtr<ID3D12Fence>
{
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    auto const hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
    if (SUCCEEDED(hr) && fence)
    {
        return fence;
    }
    return {};
}

///
/// @brief Create shared handle from resource.
///
/// @param resource 
///
auto PlatformSkiaGraphicsDeviceWin::CreateSharedHandle(Microsoft::WRL::ComPtr<ID3D12Resource> const& resource)
  -> Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>
{
    if (_d3d12Device)
    {
        if (auto const device = _d3d12Device->GetDevice())
        {
            auto handle = HANDLE();
            auto hr = device->CreateSharedHandle(resource.Get(), nullptr, GENERIC_ALL, nullptr, &handle);
            if (FAILED(hr))
            {
                handle = NULL;
            }
            return Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>(handle);
        }
    }
    return Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>();
}

///
/// @brief Create backend texture surface.
///
/// @param backendTexture Create surface with backend texture.
///
auto PlatformSkiaGraphicsDeviceWin::CreateBackendTextureSurface(GrBackendTexture const& backendTexture) -> sk_sp<SkSurface>
{
    if (_grContext)
    {
        auto resourceInfo = GrD3DTextureResourceInfo();
        if (backendTexture.getD3DTextureResourceInfo(&resourceInfo))
        {
            if (resourceInfo.fFormat == SwapChainFormat)
            {
                FW_DEBUG_ASSERT(resourceInfo.fSampleCount == SwapChainSampleDesc.Count);
                FW_DEBUG_ASSERT(resourceInfo.fSampleQualityPattern == SwapChainSampleDesc.Quality);
                return SkSurfaces::WrapBackendTexture(_grContext.get(), backendTexture, kTopLeft_GrSurfaceOrigin, resourceInfo.fSampleCount, SurfaceColorType, SkColorSpace::MakeSRGB(), nullptr);
            }
            FW_DEBUG_ASSERT(false);
        }
    }
    return nullptr;
}

///
/// @brief Flus and submit with explicit surface.
///
/// @param surface
/// @param access
/// @param sync
///
auto PlatformSkiaGraphicsDeviceWin::FlushAndSubmitSurface(SkSurface* surface, SkSurfaces::BackendSurfaceAccess access, GrSyncCpu sync) -> Bool
{
    if (_grContext)
    {
        _grContext->flush(surface, access, GrFlushInfo());
        return _grContext->submit(sync);
    }
    return false;
}

///
/// @brief Flush and submit.
///
/// @param sync
///
auto PlatformSkiaGraphicsDeviceWin::FlushAndSubmit(GrSyncCpu sync) -> Bool
{
    if (_grContext)
    {
        _grContext->flush();
        return _grContext->submit(sync);
    }
    return false;
}

///
/// @brief Signals fence object.
///
/// @param fence
/// @param nextValue
///
auto PlatformSkiaGraphicsDeviceWin::SignalFence(ID3D12Fence* fence, UINT64 nextValue) -> Bool
{
    if (_commandQueue && fence)
    {
        return SUCCEEDED(_commandQueue->Signal(fence, nextValue));
    }
    return false;
}

///
/// @brief
///
auto PlatformSkiaGraphicsDeviceWin::GetSelf() -> Shared<PlatformSkiaGraphicsDeviceWin>
{
    return _self.Lock();
}

///
/// @brief Build command queue.
///
auto PlatformSkiaGraphicsDeviceWin::BuildCommandQueue() -> Bool
{
    if (!_commandQueue && _d3d12Device)
    {
        if (auto const device = _d3d12Device->GetDevice())
        {
            auto const commandQueueDesc = D3D12_COMMAND_QUEUE_DESC {
                .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
                .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
                .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
                .NodeMask = 0,
            };
            return SUCCEEDED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(_commandQueue.GetAddressOf())));
        }
    }
    return false;
}

///
/// @brief Build GrContext.
///
auto PlatformSkiaGraphicsDeviceWin::BuildGrContext() -> Bool
{
    if (!_grContext)
    {
        auto backendContext = GrD3DBackendContext();
        backendContext.fAdapter.retain(_adapter.Get());
        backendContext.fDevice.retain(_device.Get());
        backendContext.fQueue.retain(_commandQueue.Get());
        _grContext = GrDirectContext::MakeDirect3D(backendContext);
        return _grContext != nullptr;
    }
    return false;
}

///
/// @brief
///
auto PlatformSkiaGraphicsDeviceWin::ClearResources() -> void
{
    _grContext.reset();
    _commandQueue.Reset();
    _device.Reset();
    _adapter.Reset();
}

///
/// @brief
///
auto PlatformSkiaGraphicsDeviceWin::BuildResources() -> void
{
    _adapter = _d3d12Device->GetAdapter();
    _device = _d3d12Device->GetDevice();

    if (!BuildCommandQueue())
    {
        return;
    }

    if (!BuildGrContext())
    {
        return;
    }
}
}
